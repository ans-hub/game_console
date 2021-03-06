# Game engine with software renderer

## Linear video buffer implementation

The idea of using abstract linear addressed video buffer was taken from the book of Andre LaMothe. In my implementation the linear video buffer is an abstraction consisting of an array in the system memory with a step of 4 bytes to store the pixel color in the format `ARGB (byte-ordered)`. Usually buffer has size is evaluated as *window_width x widnow_height x bytes_per_pixel*.

Hope this code helps someone to get basic knowledge before to dive into game engines. I tried to make code as simple as can without over abstraction and optimizations. Even using STL and not using memory management at all we can achive good FPS amount.

## Game engine examples

Directory `examples` contains examples based on the game engine. Currently there are games: 3d space shooter (which is my implementation of the game originally written by Andre LaMothe), game `Mountain race` and FPS game.

<div style="text-align: center;" markdown="1" />
<img src="screenshot.png" style="width: 600px;" />
</div>
<div style="text-align: center;" markdown="1" />
<img src="screenshot2.png" style="width: 600px;" />
</div>
<div style="text-align: center;" markdown="1" />
<img src="screenshot3.png" style="width: 600px;" />
</div>
<div style="text-align: center;" markdown="1" />
<img src="screenshot4.png" style="width: 600px;" />
</div>
<div style="text-align: center;" markdown="1" />
<img src="screenshot5.png" style="width: 600px;" />
</div>
<div style="text-align: center;" markdown="1" />
<img src="screenshot6.png" style="width: 600px;" />
</div>

## Libraries used in game engine

`math` - mathematical library, which I start to implement just now

`window` - performs to create window, toggle fullscreen, grab keybord events and mouse events

`draw` - draw functions, which may be used with drawing surface, given by `window` library

`audio` - is a wrapper to BASS and BASS_FX audio libraries

`system` - useful system stuffs, such as timer

`data` - data manipulation classes

`physics` - very simple physics calulations

`extras` - addons to draw lib, such as terrains, nature, birds and other

## Install

### Check dependicies:

Following packages should be installed (example given under Ubuntu 17.10):

```bash
 $ sudo apt install mesa-common-dev
 $ sudo apt install mesa-utils
 $ sudo apt install libxrandr-dev
 $ sudo apt install libglu1-mesa-dev
```

### Install `libbass.so` audio libraries (for audio module):

```bash
  $ wget http://us.un4seen.com/files/bass24-linux.zip
  $ unzip bass24-linux.zip -d tmp/
  $ sudo cp tmp/x64/libbass.so /usr/local/lib/  # for 32-bit - tmp/libbass.so
  $ sudo chmod a+rx /usr/local/lib/libbass.so
  $ sudo ldconfig
  $ rm -rf tmp/ bass24-linux.zip
```

Install `libbass_fx.so` - is the same as above (http://www.un4seen.com/files/z/0/bass_fx24-linux.zip)

P.S.: if something goes wrong while downloading bass audio library, then the best way is to use [direct link](https://www.un4seen.com/bass.html) for downloading library

### Finish the installation:

```bash
  $ git clone https://github.com/ans-hub/game_console
```

## Notes:

### Note about installation under cygwin:

This packages should be installed to work with video modes:

* cygutils-x11
* libGLU1
* xrandr
* randrproto
* libxcb-randr0

Also is necessary to add path to environment variable PATH:
```bash
  $ export PATH=/usr/local/lib:$PATH
```

Installation of bass audio library is similar, but in the case of cygwin we should download windows libraries (`bass.dll` and `bass_fx.dll`) and place it in `/usr/local/lib/`

### Note about disable FPS limiting to 60 fps

When start any example, use `vblank_mode=0 ./some_example`

### Note about fullscreen on Wayland

If you use wayland, you mas switch to old X mode to use fullscreen (i.e., in Ubuntu you may do this by choose login session called "Gnome on xorg")
