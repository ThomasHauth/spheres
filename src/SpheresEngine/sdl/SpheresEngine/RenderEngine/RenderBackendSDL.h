#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshBackend.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>

class ResourceEngine;
class Mesh;

/**
 * Render backend for SDL platforms
 */
class RenderBackendSDL: public RenderBackendBase {
public:

	/**
	 * Contstructor of the RenderBackend, initializing some configuration
	 * values
	 */
	RenderBackendSDL(ResourceEngine &);

	/**
	 * Override virtual dtor to clean up local objects
	 */
	virtual ~RenderBackendSDL() = default;

	/**
	 * Create the SDL render window
	 */
	void openDisplay() override;

	/**
	 * Destroys the SDL window
	 */
	void closeDisplay() override;

	/**
	 * Initialize OpenGL render system of SDL
	 */
	void initRenderer() override;

	/**
	 * noting done atm.
	 */
	void closeRenderer() override;

	/**
	 * Swap window with SDL swap
	 */
	void present() override;

	/**
	 * todo: remove and use directly
	 */
	void ext_glGenVertexArrays(GLuint, GLuint *) override;

	/**
	 * todo: remove and use directly
	 */
	void ext_glBindVertexArray(GLuint) override;

	/**
	 * Return a reference to the shader backend provided by this
	 * renderer
	 */
	ShaderBackend & getShaderBackend() override;

	/**
	 * Return a reference to the mesh backend of this renderer
	 */
	MeshBackend & getMeshBackend() override;

private:

	/**
	 * Pointer to the SDL window created by this
	 * renderer
	 */
	SDL_Window* m_displayWindow = nullptr;

	/**
	 * OpenGL context of SDL
	 */
	SDL_GLContext m_glContext = nullptr;

	/**
	 * Instance of the shader backend
	 */
	ShaderBackend m_shaderBackend;

	/**
	 * Instance of the mesh backend
	 */
	MeshBackend m_meshBackend;

	/**
	 * Reference to the resource engine to load meshes, textures etc.
	 */
	ResourceEngine & m_re;
};
