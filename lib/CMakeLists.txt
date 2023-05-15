cmake_minimum_required(VERSION 3.20)
project(jvm-toolbox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# get JAVA_HOME from environment variable
set(JAVA_HOME $ENV{JAVA_HOME})

# add the JNI and JVMTI include directories
include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/win32)



add_subdirectory(inspector)
add_subdirectory(injector)