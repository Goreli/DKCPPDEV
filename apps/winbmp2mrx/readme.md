# winbmp2mrx.exe

This is a console program with a simple command line interface that can read color information from a Windows Bitmap file and write it to a DKMRX file. The purpose of the program is to provide additional color data files for the [../winraster](../winraster) demonstration purposes.

Execute the program with no parameters on the command line to read its usage instructions.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know the locations of include areas: ../include;../../../libs/dkmrx/include
4. Define the real type the same as what has been used to build the dkmrx.lib library e.g. real=double.
5. Let the linker know the location of the dkmrx.lib library (add a reference to the project on the Visual C++ platform).
6. Define _CRT_SECURE_NO_WARNINGS to suppress error messages related to the use of unsafe standard library functions.

No optimisation has been used in the Debug configuration. Specifically in the Release configuration the "Favour Speed over Size" optimisation option may be desired.

## Roadmap

1. Remove dependency on <stdio.h> in module winbmpfilereader.cpp;
2. Use multithreading to read the input file and process the content;
3. Deprecate this application in favour of a more generic application that would use a uniform command line interface to convert relevant file formats including binary of some sort and delimited text - refer the DKMRX roadmap.
