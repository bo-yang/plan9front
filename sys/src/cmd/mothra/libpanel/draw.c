#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <panel.h>
#include "pldefs.h"
#define	PWID	1	/* width of label border */
#define	BWID	1	/* width of button relief */
#define	FWID	2	/* width of frame relief */
#define	SPACE	1	/* space inside relief of button or frame */
#define	CKSIZE	3	/* size of check mark */
#define	CKSPACE	2	/* space around check mark */
#define	CKWID	1	/* width of frame around check mark */
#define	CKINSET	1	/* space around check mark frame */
#define	CKBORDER 2	/* space around X inside frame */
static int plldepth;
static Image *pl_white, *pl_light, *pl_dark, *pl_black, *pl_hilit;
int pl_drawinit(int ldepth){
	plldepth=ldepth;
	pl_white=allocimage(display, Rect(0,0,1,1), screen->chan, 1, 0xFFFFFFFF);
	pl_light=allocimagemix(display, DPalebluegreen, DWhite);
	pl_dark =allocimage(display, Rect(0,0,1,1), screen->chan, 1, DPurpleblue);
	pl_black=allocimage(display, Rect(0,0,1,1), screen->chan, 1, 0x000000FF);
	pl_hilit=allocimage(display, Rect(0,0,1,1), CHAN1(CAlpha,8), 1, 0x80);
	if(pl_white==0 || pl_light==0 || pl_black==0 || pl_dark==0) return 0;
	return 1;
}
void pl_relief(Image *b, Image *ul, Image *lr, Rectangle r, int wid){
	int x, y;
	draw(b, Rect(r.min.x, r.max.y-wid, r.max.x, r.max.y), lr, 0, ZP); /* bottom */
	draw(b, Rect(r.max.x-wid, r.min.y, r.max.x, r.max.y), lr, 0, ZP); /* right */
	draw(b, Rect(r.min.x, r.min.y, r.min.x+wid, r.max.y), ul, 0, ZP); /* left */
	draw(b, Rect(r.min.x, r.min.y, r.max.x, r.min.y+wid), ul, 0, ZP); /* top */
	for(x=0;x!=wid;x++) for(y=wid-1-x;y!=wid;y++){
		draw(b, rectaddpt(Rect(0,0,1,1), Pt(x+r.max.x-wid, y+r.min.y)), lr, 0, ZP);
		draw(b, rectaddpt(Rect(0,0,1,1), Pt(x+r.min.x, y+r.max.y-wid)), lr, 0, ZP);
	}
}
Rectangle pl_boxoutline(Image *b, Rectangle r, int style, int fill){
	if(plldepth==0) switch(style){
	case UP:
		pl_relief(b, pl_black, pl_black, r, BWID);
		r=insetrect(r, BWID);
		if(fill) draw(b, r, pl_white, 0, ZP);
		else border(b, r, SPACE, pl_white, ZP);
		break;
	case DOWN:
	case DOWN1:
	case DOWN2:
	case DOWN3:
		pl_relief(b, pl_black, pl_black, r, BWID);
		r=insetrect(r, BWID);
		if(fill) draw(b, r, pl_black, 0, ZP);
		border(b, r, SPACE, pl_black, ZP);
		break;
	case PASSIVE:
		if(fill) draw(b, r, pl_white, 0, ZP);
		r=insetrect(r, PWID);
		if(!fill) border(b, r, SPACE, pl_white, ZP);
		break;
	case FRAME:
		pl_relief(b, pl_white, pl_black, r, FWID);
		r=insetrect(r, FWID);
		pl_relief(b, pl_black, pl_white, r, FWID);
		r=insetrect(r, FWID);
		if(fill) draw(b, r, pl_white, 0, ZP);
		else border(b, r, SPACE, pl_white, ZP);
		break;
	}
	else switch(style){
	case UP:
		pl_relief(b, pl_white, pl_black, r, BWID);
		r=insetrect(r, BWID);
		if(fill) draw(b, r, pl_light, 0, ZP);
		else border(b, r, SPACE, pl_white, ZP);
		break;
	case DOWN:
	case DOWN1:
	case DOWN2:
	case DOWN3:
		pl_relief(b, pl_black, pl_white, r, BWID);
		r=insetrect(r, BWID);
		if(fill) draw(b, r, pl_dark, 0, ZP);
		else border(b, r, SPACE, pl_black, ZP);
		break;
	case PASSIVE:
		if(fill) draw(b, r, pl_light, 0, ZP);
		r=insetrect(r, PWID);
		if(!fill) border(b, r, SPACE, pl_white, ZP);
		break;
	case FRAME:
		pl_relief(b, pl_white, pl_black, r, FWID);
		r=insetrect(r, FWID);
		pl_relief(b, pl_black, pl_white, r, FWID);
		r=insetrect(r, FWID);
		if(fill) draw(b, r, pl_light, 0, ZP);
		else border(b, r, SPACE, pl_white, ZP);
		break;
	}
	return insetrect(r, SPACE);
}
Rectangle pl_outline(Image *b, Rectangle r, int style){
	return pl_boxoutline(b, r, style, 0);
}
Rectangle pl_box(Image *b, Rectangle r, int style){
	return pl_boxoutline(b, r, style, 1);
}
Point pl_boxsize(Point interior, int state){
	switch(state){
	case UP:
	case DOWN:
	case DOWN1:
	case DOWN2:
	case DOWN3:
		return addpt(interior, Pt(2*(BWID+SPACE), 2*(BWID+SPACE)));
	case PASSIVE:
		return addpt(interior, Pt(2*(PWID+SPACE), 2*(PWID+SPACE)));
	case FRAME:
		return addpt(interior, Pt(4*FWID+2*SPACE, 4*FWID+2*SPACE));
	}
	return Pt(0, 0);
}
void pl_interior(int state, Point *ul, Point *size){
	switch(state){
	case UP:
	case DOWN:
	case DOWN1:
	case DOWN2:
	case DOWN3:
		*ul=addpt(*ul, Pt(BWID+SPACE, BWID+SPACE));
		*size=subpt(*size, Pt(2*(BWID+SPACE), 2*(BWID+SPACE)));
		break;
	case PASSIVE:
		*ul=addpt(*ul, Pt(PWID+SPACE, PWID+SPACE));
		*size=subpt(*size, Pt(2*(PWID+SPACE), 2*(PWID+SPACE)));
		break;
	case FRAME:
		*ul=addpt(*ul, Pt(2*FWID+SPACE, 2*FWID+SPACE));
		*size=subpt(*size, Pt(4*FWID+2*SPACE, 4*FWID+2*SPACE));
	}
}

void pl_drawicon(Image *b, Rectangle r, int stick, int flags, Icon *s){
	Rectangle save;
	Point ul, offs;
	ul=r.min;
	offs=subpt(subpt(r.max, r.min), pl_iconsize(flags, s));
	switch(stick){
	case PLACENW:	                                break;
	case PLACEN:	ul.x+=offs.x/2;                 break;
	case PLACENE:	ul.x+=offs.x;                   break;
	case PLACEW:	                ul.y+=offs.y/2; break;
	case PLACECEN:	ul.x+=offs.x/2; ul.y+=offs.y/2; break;
	case PLACEE:	ul.x+=offs.x;                   break;
	case PLACESW:	                ul.y+=offs.y;   break;
	case PLACES:	ul.x+=offs.x/2; ul.y+=offs.y;   break;
	case PLACESE:	ul.x+=offs.x;   ul.y+=offs.y;   break;
	}
	save=b->clipr;
	if(!rectclip(&r, save))
		return;
	replclipr(b, b->repl, r);
	if(flags&BITMAP) draw(b, Rpt(ul, addpt(ul, pl_iconsize(flags, s))), s, 0, ZP);
	else string(b, ul, pl_black, ZP, font, s);
	replclipr(b, b->repl, save);
}
/*
 * Place a check mark at the left end of r.  Return the unused space.
 * Caller must guarantee that r.max.x-r.min.x>=r.max.y-r.min.y!
 */
Rectangle pl_radio(Image *b, Rectangle r, int val){
	Rectangle remainder;
	remainder=r;
	r.max.x=r.min.x+r.max.y-r.min.y;
	remainder.min.x=r.max.x;
	r=insetrect(r, CKINSET);
	if(plldepth==0)
		pl_relief(b, pl_black, pl_black, r, CKWID);
	else
		pl_relief(b, pl_black, pl_white, r, CKWID);
	r=insetrect(r, CKWID);
	if(plldepth==0)
		draw(b, r, pl_white, 0, ZP);
	else
		draw(b, r, pl_light, 0, ZP);
	if(val) draw(b, insetrect(r, CKSPACE), pl_black, 0, ZP);
	return remainder;
}
Rectangle pl_check(Image *b, Rectangle r, int val){
	Rectangle remainder;
	remainder=r;
	r.max.x=r.min.x+r.max.y-r.min.y;
	remainder.min.x=r.max.x;
	r=insetrect(r, CKINSET);
	if(plldepth==0)
		pl_relief(b, pl_black, pl_black, r, CKWID);
	else
		pl_relief(b, pl_black, pl_white, r, CKWID);
	r=insetrect(r, CKWID);
	if(plldepth==0)
		draw(b, r, pl_white, 0, ZP);
	else
		draw(b, r, pl_light, 0, ZP);
	r=insetrect(r, CKBORDER);
	if(val){
		line(b, Pt(r.min.x,   r.min.y+1), Pt(r.max.x-1, r.max.y  ), Endsquare, Endsquare, 0, pl_black, ZP);
		line(b, Pt(r.min.x,   r.min.y  ), Pt(r.max.x,   r.max.y  ), Endsquare, Endsquare, 0, pl_black, ZP);
		line(b, Pt(r.min.x+1, r.min.y  ), Pt(r.max.x,   r.max.y-1), Endsquare, Endsquare, 0, pl_black, ZP);
		line(b, Pt(r.min.x  , r.max.y-2), Pt(r.max.x-1, r.min.y-1), Endsquare, Endsquare, 0, pl_black, ZP);
		line(b, Pt(r.min.x,   r.max.y-1), Pt(r.max.x,   r.min.y-1), Endsquare, Endsquare, 0, pl_black, ZP);
		line(b, Pt(r.min.x+1, r.max.y-1), Pt(r.max.x,   r.min.y  ), Endsquare, Endsquare, 0, pl_black, ZP);
	}
	return remainder;
}
int pl_ckwid(void){
	return 2*(CKINSET+CKSPACE+CKWID)+CKSIZE;
}
void pl_sliderupd(Image *b, Rectangle r1, int dir, int lo, int hi){
	Rectangle r2, r3;
	r2=r1;
	r3=r1;
	if(lo<0) lo=0;
	if(hi<=lo) hi=lo+1;
	switch(dir){
	case HORIZ:
		r1.max.x=r1.min.x+lo;
		r2.min.x=r1.max.x;
		r2.max.x=r1.min.x+hi;
		if(r2.max.x>r3.max.x) r2.max.x=r3.max.x;
		r3.min.x=r2.max.x;
		break;
	case VERT:
		r1.max.y=r1.min.y+lo;
		r2.min.y=r1.max.y;
		r2.max.y=r1.min.y+hi;
		if(r2.max.y>r3.max.y) r2.max.y=r3.max.y;
		r3.min.y=r2.max.y;
		break;
	}
	draw(b, r1, pl_light, 0, ZP);
	draw(b, r2, pl_dark, 0, ZP);
	draw(b, r3, pl_light, 0, ZP);
}
void pl_draw1(Panel *p, Image *b);
void pl_drawall(Panel *p, Image *b){
	if(p->flags&INVIS || p->flags&IGNORE) return;
	p->b=b;
	p->draw(p);
	for(p=p->child;p;p=p->next) pl_draw1(p, b);
}
void pl_draw1(Panel *p, Image *b){
	if(b!=0)
		pl_drawall(p, b);
}
void pldraw(Panel *p, Image *b){
	pl_draw1(p, b);
	flushimage(display, 1);
}
void pl_invis(Panel *p, int v){
	for(;p;p=p->next){
		if(v) p->flags|=INVIS; else p->flags&=~INVIS;
		pl_invis(p->child, v);
	}
}
Point pl_iconsize(int flags, Icon *p){
	if(flags&BITMAP) return subpt(((Image *)p)->r.max, ((Image *)p)->r.min);
	return stringsize(font, (char *)p);
}
void pl_highlight(Image *b, Rectangle r){
	draw(b, r, pl_dark, pl_hilit, ZP);
}
void pl_clr(Image *b, Rectangle r){
	draw(b, r, display->white, 0, ZP);
}
void pl_fill(Image *b, Rectangle r){
	draw(b, r, plldepth==0? pl_white : pl_light, 0, ZP);
}
void pl_cpy(Image *b, Point dst, Rectangle src){
	draw(b, Rpt(dst, addpt(dst, subpt(src.max, src.min))), b, 0, src.min);
}
