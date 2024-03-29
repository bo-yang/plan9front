#include "../port/portfns.h"

void	aamloop(int);
Dirtab*	addarchfile(char*, int, long(*)(Chan*,void*,long,vlong), long(*)(Chan*,void*,long,vlong));
void	archinit(void);
void	archreset(void);
int	bios32call(BIOS32ci*, u16int[3]);
int	bios32ci(BIOS32si*, BIOS32ci*);
void	bios32close(BIOS32si*);
BIOS32si* bios32open(char*);
void	bootargs(void*);
uintptr	cankaddr(uintptr);
int	checksum(void *, int);
void	clockintr(Ureg*, void*);
int	(*cmpswap)(long*, long, long);
int	cmpswap486(long*, long, long);
void	(*coherence)(void);
void	cpuid(int, ulong regs[]);
int	cpuidentify(void);
void	cpuidprint(void);
void	(*cycles)(uvlong*);
void	delay(int);
void*	dmabva(int);
int	dmacount(int);
int	dmadone(int);
void	dmaend(int);
int	dmainit(int, int);
#define DMAWRITE 0
#define DMAREAD 1
#define DMALOOP 2
long	dmasetup(int, void*, long, int);
void	dumpmcregs(void);
int	ecinit(int cmdport, int dataport);
int	ecread(uchar addr);
int	ecwrite(uchar addr, uchar val);
#define	evenaddr(x)				/* x86 doesn't care */
void	(*fprestore)(FPsave*);
void	(*fpsave)(FPsave*);
void	fpsserestore(FPsave*);
void	fpssesave(FPsave*);
void	fpx87restore(FPsave*);
void	fpx87save(FPsave*);
u64int	getcr0(void);
u64int	getcr2(void);
u64int	getcr3(void);
u64int	getcr4(void);
char*	getconf(char*);
void	guesscpuhz(int);
void	halt(void);
void	mwait(void*);
int	i8042auxcmd(int);
int	i8042auxcmds(uchar*, int);
void	i8042auxenable(void (*)(int, int));
void	i8042reset(void);
void	i8250console(void);
void*	i8250alloc(int, int, int);
void	i8253enable(void);
void	i8253init(void);
void	i8253reset(void);
uvlong	i8253read(uvlong*);
void	i8253timerset(uvlong);
int	i8259disable(int);
int	i8259enable(Vctl*);
void	i8259init(void);
int	i8259isr(int);
void	i8259on(void);
void	i8259off(void);
int	i8259vecno(int);
void	idle(void);
void	idlehands(void);
int	inb(int);
void	insb(int, void*, int);
ushort	ins(int);
void	inss(int, void*, int);
ulong	inl(int);
void	insl(int, void*, int);
void	intrdisable(int, void (*)(Ureg *, void *), void*, int, char*);
void	intrenable(int, void (*)(Ureg*, void*), void*, int, char*);
void	introff(void);
void	intron(void);
void	invlpg(uintptr);
void	iofree(int);
void	ioinit(void);
int	iounused(int, int);
int	ioalloc(int, int, int, char*);
int	ioreserve(int, int, int, char*);
int	iprint(char*, ...);
int	isaconfig(char*, int, ISAConf*);
void*	kaddr(uintptr);
void	kbdenable(void);
void	kbdinit(void);
KMap*	kmap(Page*);
void	kunmap(KMap*);
#define	kmapinval()
void	lgdt(void*);
void	lidt(void*);
void	links(void);
void	ltr(ulong);
void	mach0init(void);
void	mathinit(void);
void	mb386(void);
void	mb586(void);
void	meminit(void);
void	memorysummary(void);
void	mfence(void);
#define mmuflushtlb() putcr3(getcr3())
void	mmuinit(void);
uintptr	*mmuwalk(uintptr*, uintptr, int, int);
char*	mtrr(uvlong, uvlong, char *);
void	mtrrclock(void);
int	mtrrprint(char *, long);
void	mtrrsync(void);
void	noteret(void);
uchar	nvramread(int);
void	nvramwrite(int, uchar);
void	outb(int, int);
void	outsb(int, void*, int);
void	outs(int, ushort);
void	outss(int, void*, int);
void	outl(int, ulong);
void	outsl(int, void*, int);
uintptr	paddr(void*);
ulong	pcibarsize(Pcidev*, int);
void	pcibussize(Pcidev*, ulong*, ulong*);
int	pcicfgr8(Pcidev*, int);
int	pcicfgr16(Pcidev*, int);
int	pcicfgr32(Pcidev*, int);
void	pcicfgw8(Pcidev*, int, int);
void	pcicfgw16(Pcidev*, int, int);
void	pcicfgw32(Pcidev*, int, int);
void	pciclrbme(Pcidev*);
void	pciclrioe(Pcidev*);
void	pciclrmwi(Pcidev*);
int	pcigetpms(Pcidev*);
void	pcihinv(Pcidev*);
uchar	pciipin(Pcidev*, uchar);
Pcidev* pcimatch(Pcidev*, int, int);
Pcidev* pcimatchtbdf(int);
int	pcicap(Pcidev*, int);
int	pcihtcap(Pcidev*, int);
void	pcireset(void);
int	pciscan(int, Pcidev**);
void	pcisetbme(Pcidev*);
void	pcisetioe(Pcidev*);
void	pcisetmwi(Pcidev*);
int	pcisetpms(Pcidev*, int);
void	pcmcisread(PCMslot*);
int	pcmcistuple(int, int, int, void*, int);
PCMmap*	pcmmap(int, ulong, int, int);
int	pcmspecial(char*, ISAConf*);
int	(*_pcmspecial)(char *, ISAConf *);
void	pcmspecialclose(int);
void	(*_pcmspecialclose)(int);
void	pcmunmap(int, PCMmap*);
void	pmap(uintptr *, uintptr, uintptr, vlong);
void	procrestore(Proc*);
void	procsave(Proc*);
void	procsetup(Proc*);
void	procfork(Proc*);
void	putcr0(u64int);
void	putcr3(u64int);
void	putcr4(u64int);
void*	rampage(void);
int	rdmsr(int, vlong*);
void	realmode(Ureg*);
void	screeninit(void);
void	(*screenputs)(char*, int);
void*	sigsearch(char*);
void	syncclock(void);
void	syscallentry(void);
void	touser(void*);
void	trapenable(int, void (*)(Ureg*, void*), void*, char*);
void	trapinit(void);
void	trapinit0(void);
int	tas(void*);
uvlong	tscticks(uvlong*);
uintptr	umbmalloc(uintptr, int, int);
void	umbfree(uintptr, int);
uintptr	umbrwmalloc(uintptr, int, int);
void	umbrwfree(uintptr, int);
uintptr	upaalloc(int, int);
void	upafree(uintptr, int);
void	upareserve(uintptr, int);
void	vectortable(void);
void*	vmap(uintptr, int);
int	vmapsync(uintptr);
void	vunmap(void*, int);
void	wbinvd(void);
int	wrmsr(int, vlong);
int	xchgw(ushort*, int);
void	rdrandbuf(void*, ulong);

#define	userureg(ur)	(((ur)->cs & 3) == 3)
#define	waserror()	(up->nerrlab++, setlabel(&up->errlab[up->nerrlab-1]))
#define	KADDR(a)	kaddr(a)
#define PADDR(a)	paddr((void*)(a))
