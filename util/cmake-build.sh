#!/bin/bash

# make-cmake-make
# Uses Cmake to make makefiles. Execute the script in the source directory to
# create a build directory and Cmake build artefacts in it including a Makefile.
# The first argument is one of the two toolchains: Gnu (default) or Clang.
# The second argument is one of the following build types: 
#	Debug, Release (default), RelWithDebInfo or MinSizeRel

# Usage examples:
#	make-cmake-make Gnu Release
#	make-cmake-make Gnu Debug
#	make-cmake-make Clang Release
#	make-cmake-make Clang Debug

if [ -z "$1" ]
  then
    _Toolchain_=Gnu
  else
    _Toolchain_=$1
fi

if [ -z "$2" ]
  then
    _BuildType_=Release
  else
    _BuildType_=$2
fi

# Get the name of the directory the bash script lives in.
# We expect to find the CMakeClangOverrides.txt file in it.
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

mkdir -p $_Toolchain_$_BuildType_
cd $_Toolchain_$_BuildType_

if [ "$_Toolchain_" = "Clang" ]; then
	export CC=/usr/bin/clang
	export CXX=/usr/bin/clang++
	D1=-DCMAKE_USER_MAKE_RULES_OVERRIDE=$DIR/CMakeClangOverrides.txt
	D2=-D_CMAKE_TOOLCHAIN_PREFIX=llvm-
	cmake $D1 $D2 -DCMAKE_BUILD_TYPE=$_BuildType_ ..
else
	cmake -DCMAKE_BUILD_TYPE=$_BuildType_ ..
fi
