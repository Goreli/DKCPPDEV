# DKMRX.LIB
<p align="center">C++ matrix library with sample programs</p>

___

## Table of Contents
1. [Introduction](#introduction)
2. [Directory Content](#directory-content)
3. [Sample Programs](#sample-programs)
4. [Build Notes](#build-notes)
5. [Roadmap](#roadmap)

## Introduction

The purpose of the library is to provide a simple convenient programming interface for coding matrix operations. The motivation is to be able to write well understood code that intuitively maps to the Linear Algebra domain, e.g.
```C++
// Declare matrix objects.
matrix m1, m2, m3, m4;
    // Read input data.
    inStream1 >> m1;
    inStream2 >> m2;
    inStream3 >> m3;
    // Perform matrix operations.
    m4 = m1 * (m2 + m3);
    // Output the result.
    outStream << m4;
```

I wrote the first modules of the library in C when I was a uni student. I was inspired by Linear Algebra and coordinate transformation concepts back then. The functionality has transitioned to the C++ language and new features have been added since.

 An important feature underlying the implementation of overloaded operators is to properly handle temporary objects in order to preclude memory leaks. For example, the expression
```C++
m2 + m3;
```
results in a temporary matrix object. Different types of handling are required depending on whether the result is immediately assigned to another matrix object or reused as an operand in another operation. The former requires to take over the content of the temporary object. The latter requires eventual deallocation of the temporary object.

In the following example the overloaded addition operator creates a temporary object that is immediately assigned to object mrx3. Object mrx3 takes over the content of the temporary object.
```C++
matrix mrx1, mrx2, mrx3;
	inStream1 >> mrx1;
	inStream2 >> mrx2;
	// A temporary object created by the overloaded addition
	// operator is taken over by object mrx3.
	mrx3 = mrx1 + mrx2;
```

In the following example the overloaded addition operator creates a temporary object that is used as an operand by the overloaded multiplication operator. In this case the overloaded multiplication operator is responsible for deallocating the temporary object.
```C++
	// A temporary object created by the overloaded addition
	// operator is taken over by the overloaded multiplication operator.
	m4 = m1 * (m2 + m3);
```

With the advent of Move Semantics and Smart Pointers it may be possible to retire the home-grown mechanism of temporary object handling. 

Another feature worth mentioning is persistence of matrix data. The io module implements simple stream serialisation methods that allow to read/write matrix data from/to text files. The primary purpose of this functionality is to enable experimentation with relatively small chunks of data. 

It sounds like an interesting exercise to implement support for binary files as well to enable efficient i/o of large matrices. This implementation could coincide with a development of a memory mapped file i/o to enable Linear Algebra operations for large matrices that ordinarily wouldn't fit in memory.

Refer the [Roadmap](#roadmap) section in this document for the full list of roadmap items.


## Directory Content
 The root directory of the DKMRX.LIB library contains this readme.md file as well as the following subdirectories;

 * [dkmrx](dkmrx) - contains build artefacts;
 * [include](include) - contains library header files;
 * [review](review) - contains source modules currently under review or testing. These are candidate source modules that haven't made into the current set of library source modules yet;
 * [source](source) - current set of library source modules.


## Sample Programs

There are three sample programs available that demonstrate how to use the library for the purpose of coordinate transformation in computer graphics. Click on the following links to read their description:
 * Source code of [winbmp2mrx](../../apps/winbmp2mrx) is located in [../../apps/winbmp2mrx](../../apps/winbmp2mrx)
 * Source code of [wincubes](../../apps/wincubes) is located in [../../apps/wincubes](../../apps/wincubes)
 * Source code of [winraster](../../apps/winraster) is located in [../../apps/winraster](../../apps/winraster)
 
## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know the location of include area: ../include
4. Define the real type e.g. real=double.
5. Define _CRT_SECURE_NO_WARNINGS to remove "This function or variable may be unsafe" errors triggered by some string and file functions.

No optimisation has been used in the Debug configuration. Specifically in the Release configuration the "Favour Speed over Size" optimisation option may be desired.

## Roadmap

1. Transition to modern c++ e.g. smart pointers, enum class etc. Refer the content of the samples directory that has already been transitioned;
2. Transition to the Move Semantics based handling of temporary objects created by overloaded operators like *, /, + etc. There are relevant talks on Youtube that we may be able to use for the prototyping purposes:
 - https://www.youtube.com/watch?v=St0MNEU5b0o
 - https://www.youtube.com/watch?v=XS2JddPq7GQ
3. Establish a namespace. **Done. Introduced the dkmrx namespace as of 30/Oct/2019**;
4. Transition to a generic type based implementation. Should be able to choose the underlying data type using a template parameter.
5. Use Uniform Initialisation to initialise matrix objects.
6. Improve IO. Should be able to read/write delimited format files as well as *.mrx files. Add a parameter to specify the NaN->0.0 type conversion;
7. Support for large matrices. Memory mapped binary files?
8. Performance optimisation: multiplication using the L1 cache read ahead feature?
9. Performance optimisation: experiment with multithreading;
10. Gather column stats e.g. NaN counts, mean and stddev values;
11. Convenient Data Science specific interface. Shortcuts for data preprocessing/cleanup, dimensionality reduction etc;
12. Automatic testing;
13. Remove standard C library include files (e.g. stdlib.h in dkmrx_add.cpp etc). **Done. 31/Oct/2019**
