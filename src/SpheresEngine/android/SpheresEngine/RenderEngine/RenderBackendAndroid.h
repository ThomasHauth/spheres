#pragma once

//#include <android_native_app_glue.h>
#include <SpheresEngine/Util.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshBackend.h>

class ResourceEngine;

/**
 * Legacy and not used atm.
 */
class AndroidInitData {
public:
	/**
	 * Legacy and not used atm.
	 */
	AndroidInitData() = default;
};

/**
 * Legacy and not used atm.
 */
struct AndroidOpenGLES_State {
	/**
	 * Legacy and not used atm.
	 */
	EGLSurface surface;
	/**
	 * Legacy and not used atm.
	 */
	EGLContext context;

	/**
	 * Legacy and not used atm.
	 */
	EGLDisplay display;
	/**
	 * Legacy and not used atm.
	 */
	int width;
	/**
	 * Legacy and not used atm.
	 */
	int height;
};

/**
 * Legacy and not used atm.
 */
class RenderBackendAndroid: public RenderBackendBase {
public:

	/**
	 * Legacy and not used atm.
	 */
	RenderBackendAndroid(ResourceEngine & re) :
			m_re(re), m_shaderBackend(re), m_meshBackend(re, *this) {
	}

	/**
	 * Legacy and not used atm.
	 */
	void setInitData(AndroidInitData id) {
		m_initData.setValue(id);
	}

	/**
	 * Legacy and not used atm.
	 */
	void openDisplay() override;
	/**
	 * Legacy and not used atm.
	 */
	void closeDisplay() override;

	/**
	 * Legacy and not used atm.
	 */
	void initRenderer() override;
	/**
	 * Legacy and not used atm.
	 */
	void closeRenderer() override;
	/**
	 * Legacy and not used atm.
	 */
	void present() override;

	/**
	 * Legacy and not used atm.
	 */
	void ext_glGenVertexArrays(GLuint, GLuint *) override;
	/**
	 * Legacy and not used atm.
	 */
	void ext_glBindVertexArray(GLuint) override;

	/**
	 * Legacy and not used atm.
	 */
	ShaderBackend & getShaderBackend() override;
	/**
	 * Legacy and not used atm.
	 */
	MeshBackend & getMeshBackend() override;

private:
	/**
	 * Legacy and not used atm.
	 */
	util::ValidValue<AndroidInitData> m_initData;
	/**
	 * Legacy and not used atm.
	 */
	util::ValidValue<AndroidOpenGLES_State> m_state;
	/**
	 * Legacy and not used atm.
	 */
	ShaderBackend m_shaderBackend;
	/**
	 * Legacy and not used atm.
	 */
	ResourceEngine & m_re;
	/**
	 * Legacy and not used atm.
	 */
	MeshBackend m_meshBackend;
};
