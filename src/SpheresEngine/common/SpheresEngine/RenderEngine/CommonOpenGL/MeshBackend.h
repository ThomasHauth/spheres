#pragma once

//#include "OpenGLInclude.h"
#include <SpheresEngine/RenderEngine/Mesh.h>
#include <SpheresEngine/ResourceEngine/ResourceEngine.h>

class ShaderProgram;
class Texture;
class RenderBackendBase;

/**
 * Class to setup a mesh and assoicated shaders for rendering
 */
class MeshBackend {
public:
	/**
	 * Create the mseh backend
	 */
	MeshBackend(ResourceEngine & re, RenderBackendBase & renderBackend) :
			m_re(re), m_renderBackend(renderBackend) {
	}

	/**
	 * Load a mesh from file of obj file format, and setup the vertex array
	 * and store the associated texturen and shader program for easy
	 * rendering later. Once a mesh has been loaded, it will be cashed for
	 * additional calls to this method
	 */
	Mesh loadMesh(std::string name, Texture &, ShaderProgram &);

private:
	/**
	 * Caches for all loaded meshes
	 */
	std::unordered_map<std::string, Mesh> m_loadedMeshes;

	/**
	 * Reference to resource engine which is used for actually loading the mesh
	 * from disk
	 */
	ResourceEngine & m_re;

	/**
	 * The active render backend used to upload the mesh buffer data
	 */
	RenderBackendBase & m_renderBackend;
};
