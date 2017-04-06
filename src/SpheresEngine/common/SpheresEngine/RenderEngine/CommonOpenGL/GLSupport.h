#pragma once

#include <SpheresEngine/Log.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <array>
#include <string>

// define GL_CHECK_ERROR_ENABLED in build system to build
// with GL error check while debugging openGL problems
#define GL_CHECK_ERROR_ENABLED 0
#ifdef GL_CHECK_ERROR_ENABLED
#define GL_CHECK_ERROR( CALL ) {  CALL; GLSupport::checkGLError( #CALL ); }
#else
#define GL_CHECK_ERROR( CALL ) {  CALL; }
#endif

namespace GLSupport {

/**
 * Method which can be called after every OpenGL API call to check for an error in
 * the OpenGL state machine
 */
inline void checkGLError(std::string const& previousCall) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		logging::Fatal() << "glError: after " << previousCall << ": " << error;
	}
}

}

