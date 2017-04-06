#pragma once

#include <SpheresEngine/RenderEngine/OpenGLInclude.h>

/**
 * This class holds the OpenGL ids referencing the memory buffers of the mesh data
 */
class Mesh {
public:
	/**
	 * Default constructor to support storage in std::maps
	 */
	Mesh() = default;

	/**
	 * Create a mesh using the buffer id where the mesh data is stored
	 * and the vertexCount
	 */
	Mesh(GLuint vaoId, GLsizei vertexCount) :
			m_vaoId(vaoId), m_vertexCount(vertexCount) {

	}

	/**
	 * return id to OpenGL buffer holding the vertex data
	 */
	GLuint getVao() const {
		return m_vaoId;
	}

	/**
	 * Return the number of vertices in this mesh
	 */
	GLsizei getVertexCount() const {
		return m_vertexCount;
	}

private:
	/**
	 * OpenGL id referencing the memory buffer holding the mesh data
	 */
	GLuint m_vaoId = 0;

	/**
	 * Amount of vertices in this mesh
	 */
	GLsizei m_vertexCount = 0;
};
