# yolo

## What's this?
This is my 2nd attempt at developing a demotool. It is based on OpenGL.

## What is this not?
This is rather a OpenGL shader editor, than a full demomaker. It does **not compile to 64k**. This is a an editor which will create a small project file which you can play with with my upcoming player, which should compile below 64k (unpacked).

## Why "yolo"?
I was thinking for a cool name, but I couldn't come up with one, so I just said "yolo" to myself. That's why it's called yolo.

## Is it different than Bonzomatic?
Yes, it is. Bonzomatic doesn't offer custom uniforms, etc. It's just a fragment shader editor which has support for textures.

## What makes yolo unique?
yolo offers fast and live shader editing, which can be easily achieved via some json (see features) and the GUI itself. The GUI is rendered via OpenGL to prevent overhead. It also extends the default GL uniforms (resolution, etc). An example for it would be ```uniform float spectrum;```, which holds the amplitude of input audio (yes, you can inject music at runtime :smirk:).

## Shutup and gimme some screenies!
Here ya go..
![img](http://i.imgur.com/2s1fdtH.png)
[Here a video](https://streamable.com/s/y4m2x)

## Features
* Hot fragment shader reload :fire:
* Live uniform configuration :heart:
* Music injection :dancer:
* Sound visualizer :musical_note:
* Builtin logging system
* Crossplatform
* more :wink:

## TODO
* Drop JSON and move to dynamic loading
* Uniform value monitor
* Vertex injection
* Texture injection

## Dependencies
Everything is linked statically except BASS (bass.dll). Here is a list of all libraries needed to compile:

* opengl32.lib (obviously)
* nfd.lib (nativefiledialog)
* bass.lib
* glfw3.lib

Needed headers are (header's dependencies not listed):

* bass.h
* gl3w.h
* glfw3.h
* imgui.h
* nfd.h (nativefiledialog)

## Compilation
Get the dependencies, open the solution (I used VS2017), fix the include/lib paths and compile it :)

### Mac OSX
I use this command to compile it on my mac:
```bash
mkdir build
g++ main.cpp filesystem.cpp keyboard.cpp logger.cpp logmessage.cpp menubar.cpp music.cpp shaders.cpp gl3w.c imgui_draw.cpp imgui_impl_glfw_gl3.cpp imgui.cpp -o build/yolo -framework OpenGl -framework Cocoa -framework CoreVideo -framework AppKit -framework IOKit /usr/local/Cellar/glfw/3.2.1/lib/libglfw3.a ~/Desktop/lib/libnfd_d.a -I/Users/luca/Desktop/include -std=c++11 ~/Desktop/lib/libbass.dylib
cp ~/Desktop/lib/libbass.dylib ./build/libbass.dylib
```
