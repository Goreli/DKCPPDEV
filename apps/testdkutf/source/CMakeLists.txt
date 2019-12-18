#https://cmake.org/documentation
#https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
#https://stackoverflow.com/questions/6352123/multiple-directories-under-cmake

cmake_minimum_required(VERSION 3.8)

project("Test Unit Test Framework" LANGUAGES CXX)

include_directories(../include)

add_executable(testdkutf groups.cpp testdkutf.cpp utf_g110.cpp utf_g120.cpp utf_g130.cpp utf_g140.cpp)
target_compile_features(testdkutf PUBLIC cxx_std_17)

