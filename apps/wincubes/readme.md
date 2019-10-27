# wincubes.exe

This is a sample Windows Desktop application that demonstrates how to use the [../../libs/dkmrx](../../libs/dkmrx) matrix library to perform 3D coordinate transformation in **Vector Graphics** applications. It displays 9 cubes performing a dance-like motion consisting of 5 acts. The source code consists of three modules each responsible for a particular function, namely: Windows GUI, Windows **Vector Graphics** Rendering and Coordinate Transformation. The two Windows specific modules utilise the straightforward Windows API (no C++ toolkits involved). The Coordinate Transformation module is independent of OS. It is the Coordinate Transformation module that contains examples of using the library.

The demo starts as soon as the program starts - there is no dedicated user activity required to start the demo once the program has started.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know the locations of include areas: ../include;../../../libs/dkmrx/include
4. Let the resource compiler know the location of include area: ../include
5. Define the real type the same as what has been used to build the dkmrx.lib library e.g. real=double.
6. Let the linker know the location of the dkmrx.lib library (add a reference to the project on the Visual C++ platform).

## Roadmap

1. C++ 20 will introduce standard numeric constants. Replace myPi in cubes_geometry.cpp with a standard one once it's become available.
2. Can multithreading improve performance of the program?
3. Consider an OS independent graphics toolkit. Qt?
