#pragma once

#include <SpheresEngine/RenderEngine/OpenGLInclude.h>

/**
 * The data used to render a mesh
 */
struct MeshVisualData: VisualDataBase,
		LocatedVisualDataBase,
		ShadedVisualDataBase {
	/**
	 * The OpneGL id of the Vertex Array
	 */
	GLuint VertexArrayObjectId;

	/**
	 * The vertex count in the mesh
	 */
	GLsizei VertexCount;

	/**
	 * The openGL texture id to use for the diffuse part of this Mesh
	 */
	GLuint TextureId;
};
