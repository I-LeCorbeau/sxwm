###SXWM - Simple/Scriptable X Window Manager

```
Still under development. Some documentation and other stuff are lacking.
```

##Description
SXWM is a simple and scriptable window manager. By default, it doesn't do much, aside from focusing newly opened windows and handling the mouse. 
Instead, all window manipulation duties are delegated to the [sxutils](https://github.com/I-LeCorbeau/sxutils), a collection of small command line tools, and custom scripts, which can 
be tied to keybindings with the help of programs such as sxhkd or xbindkeys.  

Sample scripts and sxhkd bindings can be found in the examples sub-directory. For detailed information, start reading the man page.

##Installing
Before installing, you will need the xcb header files.

```sh
$ git clone https://github.com/I-LeCorbeau/sxwm.git
$ cd sxwm
$ make
$ sudo make clean install
```

##Setup
- Currently, sxwm is configured by editing config.h before compiling. Configuration through .Xresources (or sxwmrc) will be available in a future 
release.
- Note: config.h contains a #define OPENBSD line, which enables pledge support. Reality is that sxwm is developped on both Linux and OpenBSD, and so
depending on what system was used to make the latest commit, you will need to either comment or uncomment this line before compiling.
- A run commands file (sxwmrc) can be used to autostart programs at login, although it is highly recommend to keep it only for things that should 
be started after X, and use .xinitrc or .xsession (OpenBSD) for everything else. A sample sxwmrc, which can be copied to .config/sxwm/sxwmrc, is 
provided in the examples directory. Sxwmrc is a simple script, so you need to chmod u+x the file before using it. Being a simple script means you 
can write it in whatever scripting language you desire, though it is a shell script by default.
- Sxwmrc contains an event watcher, which can be used to execute custom commands and scripts when a given event occurs. Automatic tiling can be 
achieved with that method. Layout scripts are currently in the works.
- If the event watcher is used to set window placement and its size and color, those config parameters, along with bar size can, be set in sxwmrc 
as variables.
- A sample sxhkdrc for keybindings is provided in the examples directory.
- The examples/scripts subdirectory contains the scripts called from the sample sxhkdrc file. They can be copied to a directory available in 
$PATH (recommended dir being .local/bin), and then made executable.
- Status bars: sxwm can handle any status bar that has an override_redirect option (such as polybar). For bars that do not have such an option, 
it may be possible to use the ignw tool from sxutils to force sxwm to ignore it. This is untested currently, but will be tested at a later time.
- Workspaces: sxwm does not contain workspaces. An sxutils tool and scripts for window grouping (à la cwm) are in the works.
- Multi-monitor setups: use xrandr to handle those. The sxutils will be expanded to include monitor manipulation tools at some point.

##Guidelines

- we follow OpenBSD's KNF [style(9)](https://man.openbsd.org/style)
- code correctness

##TODO

- Code Cleanup
- Config through Xresources file
- Window grouping scripts
- Monitor Utilities (sxutils)
- Window grouping utility (sxutils)
- debug?

##Credits  

SXWM began as a fork of dcat's [swm](https://github.com/dcat/swm). It has been modified almost beyond recognition, but pieces of code remains.

