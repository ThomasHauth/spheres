cmake_minimum_required (VERSION 2.6)
project (spheres)

# CPack Setup 
SET(CPACK_GENERATOR "TGZ")

include ( CPack )

include ( CMake_Defaults.txt )

add_subdirectory( SpheresEngine )
add_subdirectory( SpheresTest )
add_subdirectory( SpheresTestBench )

enable_testing()
add_test( NAME SpheresTest 
		  COMMAND SpheresTest/SpheresTest ) 
