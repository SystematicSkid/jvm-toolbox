cmake_minimum_required(VERSION 3.20)
project(jvm-toolbox-inspector VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# get JAVA_HOME from environment variable
set(JAVA_HOME $ENV{JAVA_HOME})

# Check if JAVA_HOME is set
if(NOT JAVA_HOME)
    message(FATAL_ERROR "JAVA_HOME is not set")
endif()

# add the JNI and JVMTI include directories
include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/win32)

# Link the jni.lib and jvmti.lib files
link_directories(${JAVA_HOME}/lib)


# add the ./src directory
include_directories(src)

# gather all .hpp and .cpp files into variables
file(GLOB_RECURSE HPP_FILES src/*.hpp)
file(GLOB_RECURSE CPP_FILES src/*.cpp)

# specify that these files should be compiled to create an executable
add_library(${PROJECT_NAME} SHARED ${CPP_FILES} ${HPP_FILES})

target_link_libraries( jvm-toolbox-inspector PRIVATE jvm-toolbox-ipc-lib )