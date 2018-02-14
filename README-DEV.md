Update installation
===================

```
$ sudo apt-get update
$ sudo apt-get upgrade
```

Get OpenGL Version
==================

```
$ glxinfo | grep "OpenGL version"
OpenGL version string: 4.5.0 NVIDIA 384.111
```

Install dependencies
====================

```
sudo apt-get install build-essential mercurial make cmake autoconf automake libtool libasound2-dev libpulse-dev libaudio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev libesd0-dev libdbus-1-dev libudev-dev libgles2-mesa-dev libegl1-mesa-dev libibus-1.0-dev fcitx-libs-dev libsamplerate0-dev libsndio-dev libsamplerate0-dev libmirclient-dev libxkbcommon-dev
```

Compile SDL2
============

```
$ cd SDL2_DIR
$ ./configure
$ make
$ sudo make install
```
