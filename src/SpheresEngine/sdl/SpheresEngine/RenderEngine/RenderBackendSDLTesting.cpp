#include "RenderBackendSDLTesting.h"

#include <SpheresEngine/RenderEngine/CommonOpenGL/GLSupport.h>
#include <SpheresEngine/Log.h>

void RenderBackendSDLTesting::openDisplay() {
	logging::Info() << "Initializing SDL Testing based OpenGL";

	std::string windowName = "Spheres Engine";
	SDL_Init(SDL_INIT_VIDEO);

	// tell SDL to use no legacy OpenGL functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_displayWindow = SDL_CreateWindow(windowName.c_str(),
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

	if (!m_displayWindow) {
		m_fail = true;
		return;
	}

	m_glContext = SDL_GL_CreateContext(m_displayWindow);
	if (!m_displayWindow) {
		m_fail = true;
		return;
	}

	logging::Info() << "SDL GL Renderer created";
}

void RenderBackendSDLTesting::closeDisplay() {
	if (m_displayWindow) {
		SDL_DestroyWindow(m_displayWindow);
		m_displayWindow = nullptr;
	}
}

void RenderBackendSDLTesting::initRenderer() {

	// enable various OpenGl stuff
	/*	GL_CHECK_ERROR(glEnable(GL_TEXTURE_2D));
	 // allows to use transparent textures
	 GL_CHECK_ERROR(glEnable (GL_BLEND));
	 GL_CHECK_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	 */
	//GL_CHECK_ERROR(glClearColor(0.6f, 0.6f, 0.6f, 0.6f));
	GL_CHECK_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	// todo: get from render port init
	GL_CHECK_ERROR(glViewport(0, 0, 800, 600));

	GL_CHECK_ERROR(glClear (GL_COLOR_BUFFER_BIT));
	/*
	 //GLSupport::setupParallelProjection(width, height, -1.0f, 1.0f);

	 GL_CHECK_ERROR(glMatrixMode (GL_MODELVIEW));
	 GL_CHECK_ERROR(glLoadIdentity());*/
	GL_CHECK_ERROR(glEnable (GL_DEPTH_TEST));
	GL_CHECK_ERROR(glDepthFunc (GL_LEQUAL));

	//setup lighting model
	//GL_CHECK_ERROR(glEnable(GL_LIGHTING));
	// does GL_SMOOTH need normals for each vertex ?
	//GL_CHECK_ERROR(glShadeModel(GL_SMOOTH));
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	// w component = 0 to have purely directional light
	GLfloat light_position[] = { 0.0, -15.0, -1.0, 1.0 };
	/*
	 GL_CHECK_ERROR(glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular));
	 GL_CHECK_ERROR(glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse));
	 GL_CHECK_ERROR(glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess));*/
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//GL_CHECK_ERROR(glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0));
	/*GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	 */

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		logging::Info() << "Cannot init OpenGL EW: "
				<< glewGetErrorString(glew_status);
		m_fail = true;
		return;
	}

	if (GLEW_ARB_vertex_program) {
		logging::Info() << "OpenGL extension vertex program supported";
	} else {
		logging::Info() << "OpenGL extension vertex program not supported";
		m_fail = true;
		return;
	}

}

void RenderBackendSDLTesting::present() {
	SDL_GL_SwapWindow(m_displayWindow);
}

