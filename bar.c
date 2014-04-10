/*
 * MIT/X Consortium License
 * Copyright (c) 2014 Louis Belford
 *
 * gcc -Wall -o bar bar.h bar.c -lX11
 *
 */

#include "bar.h"


static void X_init();
static void X_cleanup();
static void render_left_colors(int, int);
static void render_right_colors(int);
static void update_output_left();
static void draw_wrk();
 
static Display *dis;
static Window win;
static GC gc;
static Colormap cm;
static Pixmap pm;
static Font txtfont;
static XFontStruct *txtfonti;
static int
    root,
    screen,
    info_prev = 0,
    prev_wrk = 0,
    screenwidth,
    screenheight,
    txtlen1,
    txtlen2,
    txtlen3,
    txtlen4;

static long
    infobg,
    wrkbg,
    wrkfg,
    focbg,
    emptybg,
    fontcol;


void
X_init() {

    int i, y;

	dis = XOpenDisplay(NULL);
	screen = DefaultScreen(dis); 		
	root = RootWindow(dis, screen);
    screenwidth = DisplayWidth(dis, screen);
	screenheight = DisplayHeight(dis, screen);

   	XColor c; 							
	long *colvs[] = { &infobg, &wrkbg, &wrkfg, &focbg, &emptybg, &fontcol, NULL};
	const char *colst[] = { INFOBG, WRKBG, WRKFG, FOCBG, EMPTYBG, FONTCOL, NULL};
	cm = DefaultColormap(dis, screen);
	for (i=0; i<6; i++) {
		XAllocNamedColor(dis, cm, colst[i], &c, &c);
		*colvs[i] = c.pixel;
	}

	txtfont = XLoadFont(dis, FONT); 	
    txtfonti = XQueryFont(dis, txtfont);

    if (LOC == 1) 							
        y = screenheight - HEIGHT;
    else
        y = 0;

	XSetWindowAttributes winat = { 			
        .override_redirect = 1,
        .colormap = cm,
        .background_pixel = emptybg,
        .event_mask = ExposureMask,
        .event_mask = PropertyChangeMask,
    };

    win = XCreateWindow(dis, root, 0, y, screenwidth, HEIGHT, 1,
        CopyFromParent,
        InputOutput,
        CopyFromParent,
        CWOverrideRedirect |
        CWColormap |
        CWBackPixel |
        CWEventMask, 
        &winat
    );

	XSetWindowBorderWidth(dis, win, 0);

    XGCValues gcv = {                       
        .background = emptybg,
        .foreground = fontcol,
        .font = txtfont,
        .graphics_exposures = 0
    };

    gc = XCreateGC(dis, root,
        GCBackground |
        GCForeground |
        GCFont |
        GCGraphicsExposures,
        &gcv
    );

	pm = XCreatePixmap(dis, root, screenwidth, HEIGHT, DefaultDepth(dis, screen));
	XSetBackground(dis, gc, emptybg);
    XSetForeground(dis, gc, fontcol);
	XMapWindow(dis, win);
    XSync(dis, False);

};

void X_cleanup() {
	XUnloadFont(dis, txtfont);              
    XFreePixmap(dis, pm);
    XFreeColormap(dis, cm);
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    exit(1);
};

void 
render_left_colors(int modelen, int wrknum) {

    int wrkx, wrf, wrloc; 
    if (modelen == 0)
        wrkx = XTextWidth(txtfonti, WRK4, strlen(WRK4)) + txtlen4 + 3;
    else
        wrkx = XTextWidth(txtfonti, WRK4, strlen(WRK4)) + txtlen4 + 9 + modelen;   

    XSetForeground(dis, gc, emptybg);                           // "clear" the window
    XFillRectangle(dis, pm, gc, 0, 0, prev_wrk, HEIGHT);
    
    XSetForeground(dis, gc, wrkbg);                             // draw workspace background
    XFillRectangle(dis, pm, gc, 0, 0, wrkx, HEIGHT);
   
    XSetForeground(dis, gc, wrkfg);                             // draw current desktop
    switch(wrknum) {
        case 0 :
            wrf = XTextWidth(txtfonti, WRK1, strlen(WRK1)) + 6;
            wrloc = 0;
            break;
        case 1 :
            wrf = XTextWidth(txtfonti, WRK2, strlen(WRK2)) + 6;
            wrloc = txtlen2 - 3;
            break;
        case 2 :
            wrf = XTextWidth(txtfonti, WRK3, strlen(WRK3)) + 6;
            wrloc = txtlen3 - 3;
            break;
        case 3 :
            wrf = XTextWidth(txtfonti, WRK4, strlen(WRK4)) + 6;
            wrloc = txtlen4 - 3;
            break;
    }
    XFillRectangle(dis, pm, gc, wrloc, 0, wrf, HEIGHT);
 
    XSetForeground(dis, gc, fontcol);
    prev_wrk = wrkx;
};

void draw_wrk() {
    XDrawString(dis, pm, gc, txtlen1, HEIGHT-HEIGHT/3, WRK1, strlen(WRK1));
    XDrawString(dis, pm, gc, txtlen2, HEIGHT-HEIGHT/3, WRK2, strlen(WRK2));
    XDrawString(dis, pm, gc, txtlen3, HEIGHT-HEIGHT/3, WRK3, strlen(WRK3));
    XDrawString(dis, pm, gc, txtlen4, HEIGHT-HEIGHT/3, WRK4, strlen(WRK4));
}

void 
render_right_colors(int buff_len) {

    int info_x;
    if (buff_len == 0) 
        info_x = 0;
    else 
        info_x = screenwidth - buff_len - 3;

    XSetForeground(dis, gc, emptybg);
    XFillRectangle(dis, pm, gc, info_prev, 0, info_prev, HEIGHT);
    
    XSetForeground(dis, gc, infobg);
    XFillRectangle(dis, pm, gc, info_x, 0, info_x, HEIGHT);
    XSetForeground(dis, gc, fontcol);

    info_prev = info_x;

}

void 
update_output_left() {

    char buffer[512], mode[32];
    int wsnum, wsm, endval, i;

    fgets(buffer, 512, stdin);
    for (i=0; i<512; i++) {
        if (buffer[i] == 'd' && buffer[i-1] == '<') {
            endval = i;
            wsnum = buffer[i+1] - '0';
            wsm = buffer[i+2] - '0';
        }
    }

    for (endval=endval-1; endval<512-endval; endval++) {
        buffer[endval] = '\0';
    }

    switch(wsm) {
        case 0 :
            sprintf(mode, "%ss-tile%s", SEPBEG, SEPEND);
            break;
        case 1 :
            sprintf(mode, "%sgrid%s", SEPBEG, SEPEND);
            break;
        case 2 :
            sprintf(mode, "%sb-tile%s", SEPBEG, SEPEND);
            break;
        case 3 :
            sprintf(mode, "%smonocle%s", SEPBEG, SEPEND);
            break;
        case 4 :
            sprintf(mode, "%sfloat%s", SEPBEG, SEPEND);
            break;
        default :
            strcpy(mode, "");
    }

    render_left_colors(XTextWidth(txtfonti, mode, strlen(mode)), wsnum);
    XDrawString(dis, pm, gc, txtlen4 + XTextWidth(txtfonti, WRK4, strlen(WRK4)) + 6, HEIGHT-HEIGHT/3, mode, strlen(mode));
    draw_wrk();

    memset(buffer, 0, 512);
    memset(mode, 0, 32); 
};

int 
main(int argc, char *argv[]) {

    int i;
    for (i=1; i<argc-1; i++) {
        if (strcmp(argv[i], "-infobg") == 0) 
            INFOBG = argv[i+1];
        if (strcmp(argv[i], "-wrkbg") == 0) 
            WRKBG = argv[i+1];
        if (strcmp(argv[i], "-wrkfg") == 0) 
            WRKFG = argv[i+1];
        if (strcmp(argv[i], "-focbg") == 0) 
            FOCBG = argv[i+1];
        if (strcmp(argv[i], "-emptybg") == 0) 
            EMPTYBG = argv[i+1];
        if (strcmp(argv[i], "-fontcol") == 0) 
            FONTCOL = argv[i+1];
        if (strcmp(argv[i], "-wrk1") == 0) 
            WRK1 = argv[i+1];
        if (strcmp(argv[i], "-wrk2") == 0) 
            WRK2 = argv[i+1];
        if (strcmp(argv[i], "-wrk3") == 0) 
            WRK3 = argv[i+1];
        if (strcmp(argv[i], "-wrk4") == 0) 
            WRK4 = argv[i+1];
        if (strcmp(argv[i], "-font") == 0) 
            FONT = argv[i+1];
        if (strcmp(argv[i], "-mbeg") == 0)
            SEPBEG = argv[i+1];
        if (strcmp(argv[i], "-mend") == 0)
            SEPEND = argv[i+1];
        if (strcmp(argv[i], "-h") == 0) {
            printf("Usage: bar [OPTION]...\n"
                   "A simple status bar and pager.\n\n"
                   "  -infobg             background colour for text on the right hand side\n"
                   "  -wrkbg              background colour for the pager\n"
                   "  -wrkfg              foreground colour for the pager\n"
                   "  -focbg              background colour for the focused window\n"
                   "  -emptybg            background colour for unused space\n"
                   "  -fontcol            colour of text\n"
                   "  -wrk[1-4]           names of the workspaces/virtual desktops\n"
                   "  -font\n"
                   "  -h or --help\n");
            exit(1);
        }
    }
   
	X_init();
    XSetForeground(dis, gc, emptybg);
    XFillRectangle(dis, pm, gc, 0, 0, screenwidth, HEIGHT);
    XSetForeground(dis, gc, fontcol);

    txtlen1 = 3;
    txtlen2 = txtlen1 + XTextWidth(txtfonti, WRK1, strlen(WRK1)) + 6;
    txtlen3 = txtlen2 + XTextWidth(txtfonti, WRK2, strlen(WRK2)) + 6;
    txtlen4 = txtlen3 + XTextWidth(txtfonti, WRK3, strlen(WRK3)) + 6;

    XEvent ev;
    XSelectInput(dis, root, PropertyChangeMask);

    char *buffer2;
    int conec_num = ConnectionNumber(dis);
    fd_set fds;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 2000000;   

    while(1) {
        FD_ZERO(&fds);
        FD_SET(conec_num, &fds);
        FD_SET(STDIN_FILENO, &fds);
        select(conec_num+1, &fds, NULL, NULL, NULL);
       
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            update_output_left();            
            XCopyArea(dis, pm, win, gc, 0, 0, screenwidth, HEIGHT, 0, 0);
            XSync(dis, False); 
        }

        if (FD_ISSET(conec_num, &fds)) {

            while (XPending(dis) !=0) {
                XNextEvent(dis, &ev);
                switch(ev.type) {
                    case PropertyNotify :
                        if (ev.xproperty.window == root && ev.xproperty.atom == XA_WM_NAME) {
                            XFetchName(dis, root, &buffer2);
                            int buff_len = XTextWidth(txtfonti, buffer2, strlen(buffer2));
                            render_right_colors(buff_len);
                            XDrawString(dis, pm, gc, screenwidth - buff_len , HEIGHT - HEIGHT/3, buffer2, strlen(buffer2));
                            XFree(buffer2);
                            XCopyArea(dis, pm, win, gc, 0, 0, screenwidth, HEIGHT, 0, 0);
                            XSync(dis, False); 
                        }
                        break;
                }
            }
        }
    }

    X_cleanup();
	return 0;
}


















