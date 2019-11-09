# DKUTF.LIB - Unit Testing Framework
<p align="center">C++ library with support for collaboration and large suites of tests.</p><p align="center">Unit test the universe.</p>


___

## Table of Contents
1. [Introduction](#introduction)
2. [Build Notes](#build-notes)
3. [Roadmap](#roadmap)

## Introduction

The purpose of the library is to provide a simple convenient programming interface in order to efficiently implement large suites of organised and well managed unit tests.

The library enables collaboration between several unit test developers. This is achieved by offering a workflow that consists of several independent tasks performed while working directly on the source code. Such separation of acitivities leads to a shorter time to market:
 - Plan and document groups of unit tests;
 - Plan, document and lay out unit tests;
 - Code unit tests;
 - Execute unit tests.

The library tracks relationship between group entities and unit tests pertaining to those entities. If there are any referential integrity issues in this relationship the library will identify and report them accordingly. 

It also provides an instrumentation tool to monitor balance between the counts of object constructor execution and object destructor execution. Also, it can detect certain cases of object corruption.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the C++ compiler know the location of include area: ../include

## Roadmap

1. Standard CLI options;
2. Implement a multilevel hierarchical structure of groups. While the lowest level of the hierarchy is hard coded in the testing application itself, it should be possible to build a custom hierarchy on top of it to enable advanced filtering of tests. This is going to be handy in large scale testing applications involving hundreds of unit tests. The plan looks like this: provide a command line option to start a GUI hierarchy editor to edit the hierarchy. Also, provide a command line option to filter (XSLT/XPATH ?) tests using the hierarchy;
3. User controlled dynamic linkage to a library of unit tests;
4. Doco.
