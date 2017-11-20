#include "RenderBackendSDL.h"

#include <SpheresEngine/RenderEngine/CommonOpenGL/GLSupport.h>
#include <SpheresEngine/Log.h>
#include <SpheresEngine/RenderEngine/RenderBackendSDLDetails.h>

RenderBackendSDL::RenderBackendSDL(ResourceEngine & re, Resolution res) :
		m_shaderBackend(re, "#version 130"), m_re(re), m_meshBackend(re, *this), m_resolution(
				res) {

}

ShaderBackend & RenderBackendSDL::getShaderBackend() {
	return m_shaderBackend;
}

MeshBackend & RenderBackendSDL::getMeshBackend() {
	return m_meshBackend;
}

void RenderBackendSDL::openDisplay() {
	logging::Info() << "Initializing SDL based OpenGL";

	std::string windowName = "Spheres Engine";
	SDL_Init (SDL_INIT_VIDEO);

	// tell SDL to use no legacy OpenGL functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_displayWindow = SDL_CreateWindow(windowName.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_resolution.w(),
			m_resolution.h(), SDL_WINDOW_OPENGL);

	m_glContext = SDL_GL_CreateContext(m_displayWindow);

	logging::Info() << "SDL GL Renderer created";
}

std::vector<std::shared_ptr<RenderBackendDetails>> RenderBackendSDL::beforeRender() {
	return {std::make_shared< RenderBackendSDLDetails >( m_resolution )};
}

void RenderBackendSDL::closeDisplay() {
	if (m_displayWindow) {
		SDL_DestroyWindow(m_displayWindow);
		m_displayWindow = nullptr;
	}
}

void RenderBackendSDL::initRenderer() {
	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		logging::Fatal() << "Cannot init OpenGL EW: "
				<< glewGetErrorString(glew_status);
	}

	if (GLEW_ARB_vertex_program) {
		logging::Info() << "OpenGL extension vertex program supported";
	} else {
		logging::Fatal() << "OpenGL extension vertex program not supported";
	}

	GL_CHECK_ERROR(glClearColor(0.0f, 0.0f, 0.6f, 1.0f));

	// Enable depth test
	GL_CHECK_ERROR(glEnable(GL_DEPTH_TEST));
	// Accept fragment if it closer to the camera than the former one
	GL_CHECK_ERROR(glDepthFunc(GL_LESS));

	GL_CHECK_ERROR(glEnable(GL_CULL_FACE));
	// counter-clockwise checking for backside checks in triangles
	GL_CHECK_ERROR(glFrontFace(GL_CCW));

}

void RenderBackendSDL::closeRenderer() {

}

void RenderBackendSDL::ext_glGenVertexArrays(GLuint i, GLuint * vaoid) {
	glGenVertexArrays(i, vaoid);
}

void RenderBackendSDL::ext_glBindVertexArray(GLuint vao) {
	glBindVertexArray(vao);
}

void RenderBackendSDL::present() {
	SDL_GL_SwapWindow(m_displayWindow);
}

