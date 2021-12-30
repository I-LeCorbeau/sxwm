###SXWM - Simple/Scriptable X Window Manager

```
This is experimental and still under development. 

```

## Description  
SXWM is a simple and scriptable window manager for X that comes with a collection of small command line utilities to manipulate windows. By default,
the sxwm binary executable only handles the mouse, takes all configure requests, and executes its configuration script (sxwmrc). Everything else is
handled by the command line utilities, shell scripts, keybindings (via sxhkd or other such programs), and the window event watcher (wew) in sxwmrc. 

Sample scripts and sxhkd bindings can be found in the examples sub-directory. For detailed information about sxwm, read the official
sxwm(1) and sxwmrc(5) man pages.

## Installing  
Before installing, you will need the xcb header files (lxcb, lxcb-util, lxcb-cursor).

```sh
$ git clone https://github.com/I-LeCorbeau/sxwm.git
$ cd sxwm
$ make
$ sudo make clean install
```
Once installed, copy the example sxwmrc to .config/sxwm and make it executable. The focus.sh and snap.sh scripts are also required for the default
sxhkdrc keybindings to work. By default, these scripts are called from .local/bin.

## Notes  
- config.h contains a #define OPENBSD line, which enables pledge support. Reality is that sxwm is developped on both Linux and OpenBSD, and so
depending on what system was used to make the latest commit, you will need to either comment or uncomment this line before compiling.
- Speaking of config.h, aside from the #define OPENBSD line, everything else should be ignored. Config.h will be removed in future versions.
- Sxwm is configured entirely through sxwmrc, include custom commands and scripts to be executed automatically when a window event occurs.
- Status bars: sxwm can handle any status bar that has an override_redirect option (such as polybar). For bars that do not have such an option, 
it may be possible to use the ignw tool to force sxwm to ignore it. This is untested currently, but will be tested at a later time.
- Workspaces: sxwm does not contain workspaces. A tool and scripts for window grouping (à la cwm) are in the works.
- Multi-monitor support is coming very soon.

## Guidelines  

- we follow OpenBSD's KNF [style(9)](https://man.openbsd.org/style)
- code correctness

## TODO  

- Code cleanup and simplification
- Window groups  
- Documentation (wiki)
- Monitor Utilities
- debug?

## Credits  

SXWM began as a fork of dcat's [swm](https://github.com/dcat/swm), and its utilities are forked from the [wmutils](https://github.com/wmutils)

