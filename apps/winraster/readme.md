# winraster.exe

This is a sample Windows Desktop application that demonstrates how to use the [../../libs/dkmrx](../../libs/dkmrx) matrix library to perform 3D coordinate transformation in **Raster Graphics** applications. It loads image color information from a *.mrx DKMRX data file and renders the image in a dance-like motion. The source code consists of three modules each responsible for a particular function, namely: Windows GUI, Windows **Raster Graphics** Rendering and Coordinate Transformation. The two Windows specific modules utilise the straightforward Windows API (no C++ toolkits involved). The Coordinate Transformation module is independent of OS. It is the Coordinate Transformation module that contains examples of using the library. 

Once the program has started it will automatically launch itself into a demo mode using a simple rectangular built-in image consisting of three colored squares. Use the File/Open menu option to navigate to the [../winbmp2mrx/sample_data](../winbmp2mrx/sample_data) directory and open the Mona_Lisa.mrx file in order to start another demo with a more interesting image displayed. Use the [../winbmp2mrx](../winbmp2mrx) program to create additional *.mrx DKMRX files to run the demo with.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know locations of include areas: ../include;../../../libs/dkmrx/include
4. Let the resource compiler know the location of include area: ../include
5. Define the real type the same as what has been used to build the dkmrx.lib library e.g. real=double.
6. Let the linker know the location of the dkmrx.lib library (add a reference to the project on the Visual C++ platform).
7. Define _CRT_SECURE_NO_WARNINGS to remove "This function or variable may be unsafe" errors triggered by some string and file functions.
8. Define _CRT_NON_CONFORMING_WCSTOK to remove wcstok/strtok related errors.

No optimisation has been used in the Debug configuration. Specifically in the Release configuration the "Favour Speed over Size" optimisation option may be desired.

## Roadmap
1. C++ 20 will introduce standard numeric constants. Replace myPi in raster_geometry.cpp with a standard one once it's become available.
2. Can multithreading improve performance of the program?
3. Consider an OS independent graphics toolkit. Qt?
4. The projection of the transformed bitmap contains blank/black pixels that appears to be a result of rounding. See if it's possible to interpolate color between the nearest neighbours to fill the blank pixels with visually meaningful values.
