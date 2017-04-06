cmake_minimum_required (VERSION 2.6)
project (SpheresTestBench)

include_directories( ${SpheresEngine_SOURCE_DIR}/../ )
include_directories( ${SpheresEngine_SOURCE_DIR}/common/ )
include_directories( ${SpheresEngine_SOURCE_DIR}/sdl/ )

set(SPHERES_USE_SDL 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBT_NO_PROFILE=1 -DUSE_SDL_OPENGL=1" )

include_directories( ${SpheresTestBench_SOURCE_DIR}/src )
include ( ${SpheresTestBench_SOURCE_DIR}/../CMake_Externals.txt )

include_directories (external/gtest/fused-src/)


add_executable(SpheresTest
	external/gtest/fused-src/gtest/gtest-all.cc
	src/test_main.cpp
	src/SpheresEngine/AnimationEngine/SequenceTest.cpp
	src/SpheresEngine/AnimationEngine/AnimationTest.cpp
	src/SpheresEngine/EntityEngineTest.cpp
	src/SpheresEngine/EntityEngineTest.cpp
	src/SpheresEngine/TimeSliceActionTest.cpp
	src/SpheresEngine/MeshRendererTest.cpp
	src/SpheresEngine/MeshLoaderTest.cpp
	src/SpheresEngine/DataTypes/StaticVectorTest.cpp
	src/SpheresEngine/DataTypes/RingBufferTest.cpp
	src/SpheresEngine/Entities/CameraEntityTest.cpp
	src/SpheresEngine/Entities/PositionedEntityTest.cpp
	src/SpheresEngine/RenderEngine/ShaderBackendTest.cpp
	src/SpheresEngine/ResourceEngine/ResourceEngineTest.cpp
	src/SpheresEngine/InputEngine/InputEngineTest.cpp
	src/SpheresEngine/PathfindingTest.cpp
)

# linking pthread for google tests 
target_link_libraries( SpheresTest pthread )
target_link_libraries( SpheresTest SpheresEngine )

