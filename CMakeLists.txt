cmake_minimum_required(VERSION 3.25)

project(spaceseptpow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # to use standard C++ rather than compiler-specific extensions

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # CMake handles creation of compile_commands.json so that my LSP doesn't throw errors

include(FetchContent)

# disable raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
# prefer static libraries for dependencies
# so the distributed game has fewer external runtime library dependencies
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW TRUE # shallow clone (no downloading the repo's ENTIRE history)
)

FetchContent_MakeAvailable(raylib)

add_executable(spaceseptpow
  src/credits.cpp
  src/keybinds.cpp
  src/main.cpp
  src/player.cpp
  src/projectile.cpp
  src/projectile-enemy-collision.cpp
  src/player-enemy-collision.cpp
  src/enemy.cpp
)

target_include_directories(spaceseptpow PRIVATE include)

# NOTE: PRIVATE = raylib is needed by spaceseptpow itself and isn't something CMake needs to propagate to targets that depend on spaceseptpow
target_link_libraries(spaceseptpow PRIVATE raylib)

if(APPLE)
    # since macOS applications are distributed as .app bundles rather than standalone executables
    set_target_properties(spaceseptpow PROPERTIES
        MACOSX_BUNDLE TRUE # package the game executable as a macOS application bundle
        MACOSX_BUNDLE_BUNDLE_NAME "SpaceSept Pow!" # human-readable application name
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.schak04.spaceseptpow" # unique identifier for the application
    )
    # since macOS stores the executable inside spaceseptpow.app/Contents/MacOS/
    # and assets are supposed to be under: spaceseptpow.app/Contents/Resources/assets/
    set(asset_output_dir "$<TARGET_FILE_DIR:spaceseptpow>/../Resources/assets")
    # install() tells CMake which files should be included when installing or packaging the application
    # TARGETS -> the executable target
    # BUNDLE treats the target as a macOS application bundle
    install(TARGETS spaceseptpow BUNDLE DESTINATION .)
    # DIRECTORY includes a directory and its contents
    install(DIRECTORY assets DESTINATION "spaceseptpow.app/Contents/Resources")
    # CPack is CMake's packaging tool
    # it takes the files specified through install() and creates distributable packages
    # macOS -> DragNDrop -> DMG
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(asset_output_dir "$<TARGET_FILE_DIR:spaceseptpow>/assets")
    # RUNTIME specifies the executable target
    install(TARGETS spaceseptpow RUNTIME DESTINATION .)
    install(DIRECTORY assets DESTINATION .)
    if(WIN32)
        # Windows -> ZIP -> ZIP
        set(CPACK_GENERATOR "ZIP")
    else()
        # Linux -> TGZ -> tar.gz
        set(CPACK_GENERATOR "TGZ")
    endif()
endif()

install(FILES PLAYER_GUIDE.txt DESTINATION .)
install(FILES README.md DESTINATION .)

# copy assets beside the executable after every successful build
# this allows the game to run directly from the build directory
# NOTE:
# POST_BUILD executes after the target finishes building
# ${CMAKE_COMMAND} -> path to the CMake executable
# -E runs one of CMake's built-in cross-platform utility commands
# make_directory creates the destination directory
# copy_directory copies the assets into that directory
add_custom_command(TARGET spaceseptpow POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${asset_output_dir}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${asset_output_dir}"
)

set(CPACK_PACKAGE_NAME "SpaceSept-Pow")
set(CPACK_PACKAGE_VENDOR "Saptaparno Chakraborty")
# CMAKE_SYSTEM_NAME: target operating system (Linux, Windows, Darwin)
# CMAKE_SYSTEM_PROCESSOR: target architecture (e.g. x86_64, aarch64)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
