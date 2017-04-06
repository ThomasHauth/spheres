end#include "RenderBackendAndroid.h"
#include <SpheresEngine/RenderEngine/CommonOpenGL/GLSupport.h>

ShaderBackend & RenderBackendAndroid::getShaderBackend() {
	return m_shaderBackend;
}

MeshBackend & RenderBackendAndroid::getMeshBackend() {
	return m_meshBackend;
}

void RenderBackendAndroid::openDisplay() {
/*	logging::Info() << "Creating OpenGL ES Surface";

	auto android_app = m_initData.getValue().getAndroidApp();
	assert(android_app);

	// check and init OpenGL ES 3.0
	if (!gl3stubInit()) {
		logging::Fatal() << "OpenGL ES 3.0 not supported on this device";
	}

	const EGLint attribs[] = { EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT, //
			EGL_BLUE_SIZE, 8, //
			EGL_GREEN_SIZE, 8, //
			EGL_RED_SIZE, 8, //
			EGL_NONE };
	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(android_app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, android_app->window,
	NULL);

	// request an OpenGL ES 3.0 context
	const EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };

	context = eglCreateContext(display, config, NULL, contextAttribs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		logging::Fatal() << "Unable to eglMakeCurrent";
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	AndroidOpenGLES_State state;

	state.display = display;
	state.context = context;
	state.surface = surface;
	state.width = w;
	state.height = h;

	logging::Info() << "Initializing OpenGL on Android with resolution " << w
			<< ":" << h;

	m_state.setValue(state);
	logging::Info() << "OpenGL initialized";*/
}

void RenderBackendAndroid::ext_glGenVertexArrays(GLuint i, GLuint * vaoid) {
	glGenVertexArrays(i, vaoid);
}

void RenderBackendAndroid::ext_glBindVertexArray(GLuint vao) {
	glBindVertexArray(vao);
}

void RenderBackendAndroid::closeDisplay() {
}

void RenderBackendAndroid::initRenderer() {
	GL_CHECK_ERROR(glClearColor(1.0f, 0.0f, 0.0f, 1.0f));

	GL_CHECK_ERROR(
			glViewport(0, 0, m_state.getValue().width,
					m_state.getValue().height));

	// enable depth buffer
	GL_CHECK_ERROR(glEnable(GL_DEPTH_TEST));
	GL_CHECK_ERROR(glDepthFunc(GL_LESS));

	GL_CHECK_ERROR(glEnable(GL_CULL_FACE));
	// counter-clockwise checking for backside checks in triangles
	GL_CHECK_ERROR(glFrontFace(GL_CCW));
}

void RenderBackendAndroid::closeRenderer() {
}

void RenderBackendAndroid::present() {
	eglSwapBuffers(m_state.get().display, m_state.get().surface);
}

