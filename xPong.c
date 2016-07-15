#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//xPong.c
//T.Lloyd 2016
typedef struct{
	int posx;
	int posy;
	int score;
}
player;
typedef struct{
	int len;
	int wid;
	char score[5];
}
gui;
void updateGame(Display *d,Window w,GC gc, player one, player two, player b,gui g){
	//Clear Window
	XClearWindow(d,w);
	//Draw Players
	XFillRectangle(d,w,gc,one.posx,one.posy,50,200);
	XFillRectangle(d,w,gc,two.posx,two.posy,50,200);
	//Draw Ball
	XFillRectangle(d,w,gc,b.posx,b.posy,50,50);
	//Draw Centre Line
	XDrawLine(d,w,gc,g.len/2,0,g.len/2,g.wid);
	//Draw Score
	XDrawString(d,w,gc,g.len-50,g.wid-50,g.score,sizeof(g.score));
}
int main(void){
	printf("Pong!\n");
	printf("T.Lloyd 2016\n");
	gui g;
	g.len=800;
	g.wid=600;
	//=====================Window Initialisation=====================
	GC gc;
	XEvent e;
	unsigned long black,white;
	Display * d = XOpenDisplay(NULL);
	int s = DefaultScreen(d);
	black=BlackPixel(d,s);
	white=WhitePixel(d,s);
	Window w=XCreateSimpleWindow(d,RootWindow(d,s), 10, 10, g.len, g.wid, 1, white, black);
	XSetStandardProperties(d,w,"xPONG","xPONG",None,NULL,0,NULL);
	XSelectInput(d,w, ExposureMask | KeyPressMask);	
	gc=XCreateGC(d,w,0,0);
	XSetBackground(d,gc,black);
	XSetForeground(d,gc,white);
	XMapWindow(d,w);
	XFlush(d);
	//Keyboard Input Stuff
	KeySym key;
	int ksize=8;
	char keys[ksize];
	keys[0]=0;
	int flag=0;
	snprintf(g.score,sizeof(g.score),"%d-%d",0,0);
	//=====================Player Initialisation=====================
	player p1,p2,ball;
	//Initialise Player 1;
	//a=up,z=down
	p1.posx=0;
	p1.posy=240;
	p1.score=0;
	//Initialise Player 2;
	//k=up,m=down
	p2.posx=g.len-50;
	p2.posy=240;
	p2.score=0;
	//How much sliders move
	int mov=50;
	//=====================Ball Initialisation=====================
	ball.posx=400;
	ball.posy=300;
	ball.score=0;	
	//Ball Speed
	int dx=1;
	int dy=1;
	int xs=1;
	int ys=1;
	//Main Game Loop
	XNextEvent(d,&e);
	while(1){
		if(XPending(d)){
			XNextEvent(d,&e);
			if(e.type==Expose&&flag==0){
				updateGame(d,w,gc,p1,p2,ball,g);
				flag=1;
			}
			if(e.type==KeyPress){
				XLookupString(&e.xkey,keys,ksize,&key,NULL);
				switch(keys[0]){
					case 97:
						p1.posy = p1.posy==0 ? p1.posy : p1.posy-mov;
						break;
					case 122:
						p1.posy = p1.posy==g.wid-200 ? p1.posy : p1.posy+mov;
						break;
					case 107:
						p2.posy = p2.posy==0 ? p2.posy : p2.posy-mov;
						break;
					case 109:
						p2.posy = p2.posy==g.wid-200 ? p2.posy : p2.posy+mov;
						break;
				}
				updateGame(d,w,gc,p1,p2,ball,g);
			}
		}
		else{
			//Ball Movement
			ball.posx+=xs*dx;
			ball.posy+=ys*dy;
			//ball reaches range of paddles
			if(((ball.posx==50)&&(ball.posy<p1.posy+200)&&(ball.posy>p1.posy))||((ball.posx==g.len-100)&&(ball.posy<p2.posy+200)&&(ball.posy>p2.posy))){
				//Paddle Hit, Change Direction
				dx*=-1;
				//Change y speed depending on where it hits the paddle
				if(((ball.posy<p1.posy+50)&&(ball.posy>p1.posy))||((ball.posy<p1.posy+200)&&(ball.posy>p1.posy+150))||((ball.posy<p2.posy+50)&&(ball.posy>p2.posy))||((ball.posy<p2.posy+200)&&(ball.posy>p2.posy+150))){
					ys = ys<4 ? ys+1 : ys;
				}
				else{
					ys = ys==1 ? ys : ys-1;
				}
			}
			else if(ball.posx<5){
				//Reset Game,adjust score etc;
				ball.posx=400;
				ball.posy=300;
				dx*=-1;
				ys=1;
				p2.score++;
			}
			else if(ball.posx>g.len-5){
				ball.posx=400;
				ball.posy=300;
				dx*=-1;
				ys=1;
				p1.score++;
			}
			else if(ball.posy<=0){
				dy*=-1;
			}
			else if(ball.posy>=g.wid-50){
				dy*=-1;
			}
			snprintf(g.score,sizeof(g.score),"%d-%d",p1.score,p2.score);
			updateGame(d,w,gc,p1,p2,ball,g);
	}
	usleep((100*100)/2);//Sleep
	}	
	XCloseDisplay(d);//Never Reachers here.
	return 0;
}