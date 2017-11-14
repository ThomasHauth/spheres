cmake_minimum_required (VERSION 2.6)
project (SpheresEngine)


set (SPHERES_ENGINE_PLATFORM_HEADER)

if (SPHERES_USE_SDL)
	message("Compiling spheres for SDL")
    set (SPHERES_ENGINE_PLATFORM_HEADER ${SpheresEngine_SOURCE_DIR}/sdl)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBT_NO_PROFILE=1 -DSPHERES_USE_SDL_OPENGL=1" )
endif()

if (SPHERES_USE_ANDROID)
	message("Compiling spheres for Android")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBT_NO_PROFILE=1 -DSPHERES_USE_ANDROID_OPENGL=1" )

    set (SPHERES_ENGINE_PLATFORM_HEADER ${SpheresEngine_SOURCE_DIR}/android)
endif()


# include this for sdl compile
include_directories( ${SpheresEngine_SOURCE_DIR}/common
    ${SPHERES_ENGINE_PLATFORM_HEADER}
)

include ( ${SpheresEngine_SOURCE_DIR}/../CMake_Externals.txt )

# common compilation units which are shared between all platforms
set(SPHERES_ENGINE_COMMON_CPP
	common/SpheresEngine/ThreadedGameLoop.cpp

	common/SpheresEngine/EntityEngine/EntityEngine.cpp
	common/SpheresEngine/EntityEngine/Entity.cpp

	common/SpheresEngine/InputEngine/InputEngine.cpp

	common/SpheresEngine/VectorTypes.cpp
	common/SpheresEngine/Log.cpp
	common/SpheresEngine/Visuals/MeshVisual.cpp
	common/SpheresEngine/Visuals/ParticleSystemVisual.cpp
	common/SpheresEngine/Visuals/ParticleModels/Milkyway.cpp

	common/SpheresEngine/Performance/SectionTimer.cpp

	common/SpheresEngine/AnimationEngine/AnimationEngine.cpp
	common/SpheresEngine/AnimationEngine/Sequence.cpp

	common/SpheresEngine/Pathfinding/Pathfinding.cpp

	common/SpheresEngine/PhysicsEngine/PhysicsEngine.cpp

	common/SpheresEngine/RenderEngine/RenderEngine.cpp
	common/SpheresEngine/RenderEngine/Texture.cpp
	common/SpheresEngine/RenderEngine/Targets/CameraTarget.cpp
	common/SpheresEngine/RenderEngine/CommonOpenGL/MeshRenderer.cpp
	common/SpheresEngine/RenderEngine/CommonOpenGL/ParticlesRenderer.cpp
	common/SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.cpp
	common/SpheresEngine/RenderEngine/CommonOpenGL/MeshBackend.cpp

	common/SpheresEngine/ResourceEngine/ResourceEngineAbstract.cpp
	common/SpheresEngine/ResourceEngine/MeshLoader.cpp
)

set(SPHERES_ENGINE_SDL_CPP)

if (SPHERES_USE_SDL)
set(SPHERES_ENGINE_SDL_CPP
	# SDL platform specific compilation units
	sdl/SpheresEngine/ResourceEngine/ResourceEngine.cpp
	sdl/SpheresEngine/RenderEngine/RenderBackendSDL.cpp
	sdl/SpheresEngine/RenderEngine/RenderBackendSDLTesting.cpp
	sdl/SpheresEngine/InputEngine/SdlSource.cpp
)
	if (UNIX)
		message("Enabled inotify support")
		set(SPHERES_ENGINE_SDL_CPP 
		    ${SPHERES_ENGINE_SDL_CPP}
		    "external/inotify-cxx/inotify-cxx.cpp")
	endif()
endif()

if (SPHERES_USE_ANDROID)
set(SPHERES_ENGINE_ANDROID_CPP
	# ANDROID specific compilation units
	android/SpheresEngine/ResourceEngine/ResourceEngine.cpp
	android/SpheresEngine/RenderEngine/RenderBackendAndroidVR.cpp
	android/SpheresEngine/AndroidVRFramework.cpp
	android/SpheresEngine/RenderEngine/Targets/VREyeTarget.cpp
	android/SpheresEngine/InputEngine/DaydreamController.cpp
)
endif()

## external libraries which will directly compiled in the Engine for 
# simplicity and better optimization
file(GLOB_RECURSE SPHERES_ENGINE_BULLET_CPP external/bullet/src/*.cpp)

add_library(SpheresEngine STATIC #SHARED
    # common cpps
	${OPTIONAL_CPP}
    ${SPHERES_ENGINE_COMMON_CPP}
    # externals directly compiled into the engine lib
    ${SPHERES_ENGINE_BULLET_CPP}
    # platform specific
    ${SPHERES_ENGINE_SDL_CPP}
    ${SPHERES_ENGINE_ANDROID_CPP}
)

# other platform compiles will do linking outside of this cmake file
if (SPHERES_USE_SDL)
    target_link_libraries( SpheresEngine SDL2 SDL2_image GLEW pthread boost_system boost_filesystem )

    if(UNIX)
	    target_link_libraries( SpheresEngine GL )
    endif()
endif()

if (SPHERES_USE_ANDROID)
    target_link_libraries( SpheresEngine log android EGL GLESv3)
endif()

