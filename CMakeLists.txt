cmake_minimum_required(VERSION 3.25)

project(spaceseptpow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # to use standard C++ rather than compiler-specific extensions

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # disable raylib examples

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW TRUE # shallow clone (no downloading the repo's ENTIRE history)
)

FetchContent_MakeAvailable(raylib)

add_executable(spaceseptpow src/main.cpp)

# NOTE: PRIVATE = raylib is needed by spaceseptpow itself and isn't something CMake needs to propagate to targets that depend on spaceseptpow
target_link_libraries(spaceseptpow PRIVATE raylib) 
