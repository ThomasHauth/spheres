cmake_minimum_required (VERSION 2.6)
project (SpheresTestBench)

include_directories( ${SpheresEngine_SOURCE_DIR}/../ )
include_directories( ${SpheresEngine_SOURCE_DIR}/common/ )
include_directories( ${SpheresEngine_SOURCE_DIR}/sdl/ )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBT_NO_PROFILE=1 -DUSE_SDL_OPENGL=1" )


include_directories( ${SpheresTestBench_SOURCE_DIR}/src )
include ( ${SpheresTestBench_SOURCE_DIR}/../CMake_Externals.txt )

set(SPHERES_USE_SDL 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBT_NO_PROFILE=1 -DUSE_SDL_OPENGL=1" )

add_executable(SpheresTestBench
	src/main.cpp
)

target_link_libraries( SpheresTestBench SpheresEngine )

