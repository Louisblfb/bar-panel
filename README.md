
BAR
===

Bar is a simple status bar and pager. It is a work in progress so expect nothing. 

FEATURES
========

The pager displays four named virtual desktops or "workspaces", beyond that they are represented by a number. If you use monsterwm it will also display the current mode. As with many other panels/status bars, status information (time, battery, etc) is set through the root window name. A simple shell script can accomplish this. Desktop information is piped into the bar through stdin. You will need a so called "filter" to format this information. 

RUN
===

If you use a WM that supports EMWH, I have written a program that formats the value returned by _NET_ACTIVE_DESKTOP into something understanderble to the bar. If you use monsterwm, pipe its output into monst-parse and then into the bar. Bar has the following command line options:

-infofg [color]
-wrkbg [color]
-wrkfg [color]
-emptybg [color]
-fontcol [color]
-wrk[1-4] [string]
-font [xfont string]

Start Bar with either of the following (add to your xinit script):

emwh-parse | bar

or 

monsterwm | monst-parse | bar

A simple script to display the time:

while [ 1 ]
    do
        xsetroot -name "[$(date +%X)] "
        sleep 1
    done

INSTALLATION
============

To compile bar you will need xlib. First edit config.h to your liking and then type make. 


