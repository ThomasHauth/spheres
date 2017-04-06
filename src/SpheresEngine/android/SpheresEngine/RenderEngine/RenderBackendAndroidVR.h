#pragma once

#include <SpheresEngine/Util.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshBackend.h>

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_types.h>

class ResourceEngine;

/**
 * Render Backend for the Android VR API
 */
class RenderBackendAndroidVR: public RenderBackendBase {
public:

	/**
	 * Create the render backend and store the resource engine
	 * reference for later usage
	 */
	RenderBackendAndroidVR(ResourceEngine & re) :
			m_re(re), m_shaderBackend(re), m_meshBackend(re, *this) {
	}

	/**
	 * No work required here, is done by the Android Java Activity
	 */
	void openDisplay() override {
	}

	/**
	 * No work required here, is done by the Android Java Activity
	 */
	void closeDisplay() override {
	}

	/**
	 * Initialize OpenGL and configure render settings
	 */
	void initRenderer() override;

	/**
	 * No work required
	 */
	void closeRenderer() override {
	}

	/**
	 * Submits the frame for display by the Google VR API
	 */
	void present() override;

	/**
	 * todo: remove and call directly
	 */
	void ext_glGenVertexArrays(GLuint, GLuint *) override;

	/**
	 * todo: remove and call directly
	 */
	void ext_glBindVertexArray(GLuint) override;

	/**
	 * Returns the shader backend
	 */
	ShaderBackend & getShaderBackend() override;

	/**
	 * Returns the Mesh backend
	 */
	MeshBackend & getMeshBackend() override;

	/**
	 * Retrieves and returns the eye views from GVR
	 */
	std::vector<RenderBackendDetails*> beforeRender() override;

	/**
	 * Update the pointer to the GVR Api. This is called by the Android VR Framework
	 * as soon as it is available
	 */
	void setGvrObjects(gvr::GvrApi * gvrApi) {
		m_gvr_api = gvrApi;
	}

private:
	/**
	 * Makes sure the frame buffer is properly sized if the target
	 * size changed
	 */
	void PrepareFramebuffer();

	/**
	 * Use default shader backend
	 */
	ShaderBackend m_shaderBackend;

	/**
	 * Holds reference to resource engine
	 */
	ResourceEngine & m_re;

	/**
	 * Use default mesh backend
	 */
	MeshBackend m_meshBackend;

	/**
	 * Pointer to GVR API
	 */
	gvr::GvrApi * m_gvr_api = nullptr;

	/**
	 * Viewports for rendering
	 */
	std::unique_ptr<gvr::BufferViewportList> m_viewport_list;

	/**
	 * GVR Swap chain
	 */
	std::unique_ptr<gvr::SwapChain> m_swap_chain;

	/**
	 * Size of the render target
	 */
	gvr::Sizei m_render_size;

	/** CLass used to render in the current frame
	 * is acquired in beforePresent()
	 */
	gvr::Frame m_frame = gvr::Frame(nullptr);

	/**
	 * Latest head view matrix. Needed to compute the two eye view matrices and
	 * to submit the rendered frame
	 */
	gvr::Mat4f m_head_view;

	/**
	 * Scratch viewport for rendering
	 */
	std::unique_ptr<gvr::BufferViewport> m_scratch_viewport;
};

