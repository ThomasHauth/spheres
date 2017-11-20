#pragma once

#include <boost/noncopyable.hpp>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/Mesh.h>
#include <string>
#include <memory>
#include <vector>
#include <utility>

class MeshBackend;
class ShaderBackend;

/**
 * This class can derived from to transfer backend-specific data from
 * the render backend to the render targets and specific renderers
 */
class RenderBackendDetails {
public:

	/**
	 * Implement default dtor to support inheritance
	 */
	virtual ~RenderBackendDetails() = default;

};

/**
 * Class (to derive from) which the backend implemenatinos
 * can use to forward data to targets & renderers
 */
class RenderBackendBase: boost::noncopyable {
public:

	/**
	 * Implement default dtor to support inheritance
	 */
	virtual ~RenderBackendBase() {

	}

	/**
	 * Called when then backend should open the display
	 */
	virtual void openDisplay() {

	}

	/**
	 * Called when then backend should close the display
	 */
	virtual void closeDisplay() {

	}

	/**
	 * Called when then backend should init the renderer
	 */
	virtual void initRenderer() = 0;

	/**
	 * Called when then backend clean up the renderer code
	 */
	virtual void closeRenderer() {

	}

	/**
	 * Called when then Renderers are done the Backend can present
	 * the rendered image to the user
	 */
	virtual void present() {

	}

	/**
	 * Called before any target is rendered. Allows the backend to
	 * provide updated backend details to the render targets
	 */
	virtual std::vector< std::shared_ptr<RenderBackendDetails>> beforeRender() {
		return {};
	}

	/**
	 * Returns a reference to the shader backend provided by this
	 * render backend
	 */
	virtual ShaderBackend & getShaderBackend() = 0;

	/**
	 * Retruns a reference to the mesh backend provided by this
	 * render backend
	 */
	virtual MeshBackend & getMeshBackend() = 0;

	/**
	 * todo: remove and use directly
	 */
	virtual void ext_glGenVertexArrays(GLuint, GLuint *) = 0;

	/**
	 * todo: remove and use directly
	 */
	virtual void ext_glBindVertexArray(GLuint) = 0;

};
