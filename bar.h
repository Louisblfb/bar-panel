/* 
 * MIT/X Consortium License
 * Copyright (c) 2014 Louis Belford
 *
 * gcc -Wall -o bar bar.h bar.c -lX11
 *
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <stdlib.h>
#include <stdio.h>

static int 
    HEIGHT    =   14,
    LOC       =   1,    // LOC defines the location of the bar (0-top, 1-bottom)
    PADDING   =   24,
    GAP       =   6;
   
static char
    *WRK1     =    " 1 ",
    *WRK2     =    " 2 ",
    *WRK3     =    " 3 ",
    *WRK4     =    " 4 ",
    *FONT     =    "-benis-lemon-medium-r-normal--10-110-75-75-m-50-iso8859-1",
    *SEPBEG   =    "[",
    *SEPEND   =    "] ",

    *INFOBG   =    "#333333",
    *WRKBG    =    "#333333",
    *WRKFG    =    "#1f1f22",
    *EMPTYBG  =    "#1f1f22",
    *FONTCOL  =    "#666666";

