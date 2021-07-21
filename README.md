# Ved's build of dwm

dwm is an extremely fast, small, and dynamic window manager for X.


## Bindings
Custom bindings are commented in the config headers [config.def.h](config.def.h).

## Patches 

All patches applied to this build are stored in /patch

 - autostart: loads up commands in ~/.dwm/autostart.sh
 - bottomstack: a horizontal stack layout `super + shift + t`
 - fadeinactive: fades inactive window by reducing opacity
 - fakefullscreen: clients can only fullscreen into the space given to them. To be used with fullscreen below
 - fullscreen: force a window to go fullscreen and hide the statusbar `super + f`
 - hide\_vacant\_tags: hide tags with no windows for a cleaner bar
 - nametag: allow custom text to be appended to tag numbers `super + n`
 - pertag: allow setting different layouts for unique tags
 - restartsig: kill & restart dwm without logging out - useful post compilation. `super + ctrl + shift + q`
 - scratchpad: spawn a floating terminal over any window with `super + shift + enter`
 - stacker: utilities to  manage the client stack. `super` for push operations and `super+shift` for focus operations
 - sticky: set a window as sticky to all tags `super + s`
 - systray: a system tray implementation in dwm
 - xresources: compatibility with settings in .xresources

## Requirements
In order to build dwm you need the Xlib header files. These should be available in your distribution's Xorg package.

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
