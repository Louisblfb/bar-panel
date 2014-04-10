
BAR
===

Bar is a simple status bar and pager. Bar currently works with WMs that support EWMH. It also works with Monsterwm. 

FEATURES
========

The pager displays four named virtual desktops or "workspaces", beyond that they are represented by a number. If you use monsterwm it will also display the current mode. As with many other panels/status bars, status information (time, battery, etc) is set through the root window name. A simple shell scripts can accomplish this. Desktop information is piped into the bar through stdin. You will need a so called "filter" to format this information. 

RUN
===

If you use a WM that supports EMWH, I have written a program that formats the value returned by NET_ACTIVE_DESKTOP into something understanderble to the bar. If you use monsterwm, pipe its output into monst-parse and then into the bar. It has the following command line options:

-infofg 
-wrkbg
-wrkfg
-emptybg
-fontcol
-wrk[1-4]

Start with either of the following:

emwh-parse | bar

or 

monsterwm | monst-parse | bar

INSTALLATION
============

To compile bar you will need xlib. First edit config.h to your liking and then type make. 


