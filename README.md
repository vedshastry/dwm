# Ved's build of dwm

dwm is an extremely fast, small, and dynamic window manager for X.


## Bindings
Custom bindings are commented in the config headers [config.def.h](config.def.h).

## Patches 

All patches applied to this build are stored in /patch

 - pertag: allow setting different layouts for unique tags
 - restartsig: kill & restart dwm without logging out - useful post compilation. `super + ctrl + shift + q`
 - sticky: set a window as sticky to all tags `super + s`
 - xresources: compatibility with settings in .xresources
 - nametag: allow custom text to be appended to tag numbers `super + n`
 - fullscreen: force a window to go fullscreen and hide the statusbar `super + f`
 - systray: a system tray implementation in dwm
 - bottomstack: a horizontal stack layout `super + shift + t`
 - fadeinactive: fades inactive window by reducing opacity
 - hide\_vacant\_tags: hide tags with no windows for a cleaner bar
 - stacker: utilities to  manage the client stack. `super` for push operations and `super+shift` for focus operations
 - scratchpad: spawn a floating terminal over any window with `super + shift + enter`
 - autostart: loads up commands in ~/.dwm/autostart.sh

## Requirements
In order to build dwm you need the Xlib header files. There should be available in your distribution's xorg package.

## Installation

Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

```
git clone https://github.com/shastryv/dwm
cd dwm
rm config.h && sudo make clean install
```


## Usage
Add the following line to your .xinitrc to start dwm using startx:

```
    exec dwm
```
