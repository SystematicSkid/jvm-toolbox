cmake_minimum_required(VERSION 3.20)

project(jvm-toolbox)

# Define nominmax for windows
add_definitions(-DNOMINMAX)

# Include directories
include_directories(includes)

add_subdirectory(lib)
add_subdirectory(shared)

add_custom_target(java-example-hello-world ALL COMMAND gradle -p ../examples/hello_world run)
# Build jar file
add_custom_target(java-example-hello-world-jar ALL COMMAND gradle -p ../examples/hello_world jar)