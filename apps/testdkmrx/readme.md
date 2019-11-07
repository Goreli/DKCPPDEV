# testdkmrx.exe

This is a console application that executes a suite of unit tests to test functionality of the [../../libs/dkmrx](../../libs/dkmrx) matrix library. Uses the [../../libs/dkutest](../../libs/dkutest) library for the instrumentation purposes.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know locations of include areas: ../include;../../../libs/dkmrx/include;../../../libs/dkutest/include
4. Define the real type the same as what has been used to build the dkmrx.lib library e.g. real=double.
5. Let the linker know the location of the dkutest.lib library (add a reference to the project on the Visual C++ platform).
6. Don't link the dkmrx library. Rather compile the dkmrx source files directly into the project. This is to preclude a potential confusion that may be caused to other applicatins due to the use of the [../../libs/dkutest](../../libs/dkutest) instrumentation.
7. Define DKMRX_MATRIX_TEST to enable the [../../libs/dkutest](../../libs/dkutest) instrumentation.
8. Define _CRT_SECURE_NO_WARNINGS to remove "This function or variable may be unsafe" errors triggered by some string and file functions.


## Roadmap
1. Need command line options to navigate the groups of tests available.
