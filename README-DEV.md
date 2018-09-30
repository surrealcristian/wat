# Update installation

```
$ sudo apt-get update
$ sudo apt-get upgrade
```

# Useful files

```
SDL2-2.0.8/docs/README-linux.md
SDL2-2.0.8/INSTALL.txt
```

# Install dependencies

```
sudo apt-get install build-essential mercurial make cmake autoconf automake libtool libasound2-dev libpulse-dev libaudio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev libdbus-1-dev libudev-dev libgles2-mesa-dev libegl1-mesa-dev libibus-1.0-dev fcitx-libs-dev libsamplerate0-dev libsndio-dev libwayland-dev libxkbcommon-dev wayland-protocols libmirclient-dev libxkbcommon-dev
```

# Get OpenGL Version

```
$ glxinfo | grep "OpenGL version"
OpenGL version string: 4.5.0 NVIDIA 384.111
```

# Compile SDL2

```
$ cd SDL2-2.0.8/
$ ./configure
$ make
$ sudo make install
```

# Research

```
SDL_EventState
SDL_GetKeyboardState()
SDL2.0.6 Stuttering Problem
```
