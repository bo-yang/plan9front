#include <u.h>
#include <libc.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>

enum {
	NBUF = 8*1024,
	NDELAY = 2048,
	NCHAN = 2,
};

typedef struct Stream Stream;
struct Stream
{
	int	used;
	int	run;
	ulong	wp;
	QLock;
	Rendez;
};

ulong	mixrp;
int	mixbuf[NBUF][NCHAN];
Stream	streams[16];

int
s16(uchar *p)
{
	int v;

	v = p[0]<<(sizeof(int)-2)*8 | p[1]<<(sizeof(int)-1)*8;
	v >>= (sizeof(int)-2)*8;
	return v;
}

int
clip16(int v)
{
	if(v > 0x7fff)
		return 0x7fff;
	if(v < -0x8000)
		return -0x8000;
	return v;
}

void
fsopen(Req *r)
{
	Stream *s;

	if(strcmp(r->fid->file->name, "audio") != 0){
		respond(r, nil);
		return;
	}
	for(s = streams; s < streams+nelem(streams); s++){
		qlock(s);
		if(s->used == 0 && s->run == 0){
			s->used = 1;
			qunlock(s);

			r->fid->aux = s;
			respond(r, nil);
			return;
		}
		qunlock(s);
	}
	respond(r, "all streams in use");
}

void
fsclunk(Fid *f)
{
	Stream *s;

	if(f->file != nil && strcmp(f->file->name, "audio") == 0 && (s = f->aux) != nil){
		f->aux = nil;
		s->used = 0;
	}
}

void
audioproc(void *)
{
	static uchar buf[NBUF*NCHAN*2];
	int nsleep, fd, i, j, n, m, v;
	Stream *s;
	uchar *p;

	threadsetname("audioproc");

	fd = -1;
	nsleep = 0;
	for(;;){
		m = NBUF;
		for(s = streams; s < streams+nelem(streams); s++){
			qlock(s);
			if(s->run){
				n = (long)(s->wp - mixrp);
				if(n <= 0 && nsleep > 4)
					s->run = 0;
				else if(n < m)
					m = n;
				if(n < NDELAY)
					rwakeup(s);
			}
			qunlock(s);
		}
		m %= NBUF;

		if(m == 0){
			sleep(1<<nsleep);
			if(nsleep < 7)
				nsleep++;
			else {
				close(fd);
				fd = -1;
			}
			continue;
		}
		if(fd < 0)
			if((fd = open("/dev/audio", OWRITE)) < 0)
				continue;

		nsleep = 0;

		p = buf;
		for(i=0; i<m; i++){
			for(j=0; j<NCHAN; j++){
				v = clip16(mixbuf[mixrp % NBUF][j]);
				mixbuf[mixrp % NBUF][j] = 0;
				*p++ = v & 0xFF;
				*p++ = v >> 8;
			}
			mixrp++;
		}
		write(fd, buf, p - buf);
	}
}

void
fswrite(Req *r)
{
	Srv *srv;
	int i, j, n, m;
	Stream *s;
	uchar *p;

	p = (uchar*)r->ifcall.data;
	n = r->ifcall.count;
	m = n/(NCHAN*2);

	srv = r->srv;
	srvrelease(srv);
	s = r->fid->aux;
	qlock(s);
	if(s->run == 0){
		s->wp = mixrp;
		s->run = 1;
	}
	for(i=0; i<m; i++){
		while((long)(s->wp - mixrp) >= NBUF-1){
			s->run = 1;
			rsleep(s);
		}
		for(j=0; j<NCHAN; j++){
			mixbuf[s->wp % NBUF][j] += s16(p);
			p += 2;
		}
		s->wp++;
	}
	if((long)(s->wp - mixrp) >= NDELAY){
		s->run = 1;
		rsleep(s);
	}
	qunlock(s);
	r->ofcall.count = n;
	respond(r, nil);
	srvacquire(srv);
}

void
fsstart(Srv *)
{
	Stream *s;

	for(s=streams; s < streams+nelem(streams); s++){
		s->used = s->run = 0;
		s->Rendez.l = &s->QLock;
	}
	proccreate(audioproc, nil, 16*1024);
}

void
fsend(Srv *)
{
	threadexitsall(nil);
}

Srv fs = {
	.open=		fsopen,
	.write=		fswrite,
	.destroyfid=	fsclunk,
	.start=		fsstart,
	.end=		fsend,
};

void
usage(void)
{
	fprint(2, "usage: %s [-D] [-s srvname] [-m mtpt]\n", argv0);
	exits("usage");
}

void
threadmain(int argc, char **argv)
{
	char *srv = nil;
	char *mtpt = "/mnt/mix";

	ARGBEGIN{
	case 'D':
		chatty9p++;
		break;
	case 's':
		srv = EARGF(usage());
		break;
	case 'm':
		mtpt = EARGF(usage());
		break;
	default:
		usage();
	}ARGEND;

	if(argc)
		usage();

	fs.tree = alloctree(nil, nil, DMDIR|0777, nil);
	createfile(fs.tree->root, "audio", nil, 0222, nil);
	threadpostmountsrv(&fs, srv, mtpt, MREPL);

	mtpt = smprint("%s/audio", mtpt);
	if(bind(mtpt, "/dev/audio", MREPL) < 0)
		sysfatal("bind: %r");
	free(mtpt);

	threadexits(0);
}