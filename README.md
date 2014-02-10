# yeti3d-sdl
[![Build Status](https://secure.travis-ci.org/klaussilveira/yeti3d-sdl.png)](http://travis-ci.org/klaussilveira/yeti3d-sdl)

Yeti3D is a software-rendered 3D game engine, created by Derek J. Evans. It's primary focus is portability, being used in many ports for different platforms, specially game consoles like 32X, N64, DC, NDS and others. This fork is based on the latest know release of the PRO version. It has been cleaned up and fixed, although there are still many bugs in the code.

This fork has the objective of: 

* Laying the foundation for solid ports to many different platforms
* Speed up the engine and clean game-specific behavior
* Using a modern SDL2 implementation as reference

## TODO
* Get the editor running
* Implement DC using KallistiOS 2
* Implement N64 using libdragon
* Get rid of warnings, speed up pixel_buffer_draw and pixel_converter
