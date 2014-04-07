/* 
 * MIT/X Consortium License
 * Copyright (c) 2014 Louis Belford
 *
 * gcc -Wall -o bar bar<version>.h bar<version>.c -lX11
 *
 * This file is used to set the default colors,
 * desktop names, font and size.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdlib.h>
#include <stdio.h>

static int 
    HEIGHT    =   14,
    LOC       =   1;    // LOC defines the location of the bar (0-top, 1-bottom)
   
static char
    *WRK1     =    " 1 ",
    *WRK2     =    " 2 ",
    *WRK3     =    " 3 ",
    *WRK4     =    " 4 ",
    *FONT     =    "-benis-lemon-medium-r-normal--10-110-75-75-m-50-iso8859-1",
    *SEPBEG   =    " ",
    *SEPEND   =    " ",

    *INFOBG   =    "#22222b"/*"#0d020a"/"#333333"*/,
    *WRKBG    =    "#22222b"/*"#0d020a"/"#333333"*/,
    *WRKFG    =    "#101019"/*"#030514"/"#1f1f22"*/,
    *FOCBG    =    "#101019"/*"#030514"/"#1f1f22"*/,
    *EMPTYBG  =    "#101019"/*"#030514"/"#1f1f22"*/,
    *FONTCOL  =    "#333333"/*"#666666"*/;

