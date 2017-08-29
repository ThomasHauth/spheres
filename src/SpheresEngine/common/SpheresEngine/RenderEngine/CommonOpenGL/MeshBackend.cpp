#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshBackend.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>
#include "../../Log.h"

#include <cstring>
#include <ostream>

Mesh MeshBackend::loadMesh(std::string name, Texture &,
		ShaderProgram & shaderProg) {

	auto it = m_loadedMeshes.find(name);
	if (it != m_loadedMeshes.end())
		return it->second;

	// load mesh from disk
	const auto meshData = m_re.loadMesh(name);

	GLuint gVAO;
	GLuint gVBO;

	// make and bind the VAO
	// this allows to capture a state of the whole
	// vertex setup below and can be bound to
	// have the exact same settings as done here
	m_renderBackend.ext_glGenVertexArrays(1, &gVAO);
	m_renderBackend.ext_glBindVertexArray(gVAO);

	// make and bind the VBO
	// holds the vertex data on the GPU
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Put the three triangle vertices into the VBO
	const auto facesData = meshData.asXYZUVNormals();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * facesData.size(),
			facesData.data(), GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader with the uv and
	// normal information and dump it all in one buffer. This buffer has 8 entries/vertex
	// xyz + uz + 3dim normals

	// todo: will this work if we have the same mesh with mulitple shader programs !?!

	const size_t combinedBufferEntryCount = 8;
	auto combinedBufferSize = combinedBufferEntryCount * sizeof(GLfloat);

	// pass on vertex position
	glEnableVertexAttribArray(shaderProg.getAttribLocation("vert"));
	glVertexAttribPointer(shaderProg.getAttribLocation("vert"),	// vertex pos
	3,	// will be three (x,y,z) numbers
			GL_FLOAT,	// of float
			GL_FALSE,	// not normalized
			combinedBufferSize,	// stride (in bytes) is the # of elemnts
			NULL);

	// pass on vertex texture coordinates
	glEnableVertexAttribArray(shaderProg.getAttribLocation("vertTexCoord"));
	glVertexAttribPointer(shaderProg.getAttribLocation("vertTexCoord"),	// uv coords
	2,	// will have two entries
			GL_FLOAT, GL_TRUE, combinedBufferSize,
			/* The offset within the buffer is 3, because the xyz coords come first*/
			(const GLvoid*) (3 * sizeof(GLfloat)));

	// pass on vertex normals
	glEnableVertexAttribArray(
			shaderProg.getAttribLocation("normals_modelspace"));
	glVertexAttribPointer(shaderProg.getAttribLocation("normals_modelspace"),
	/* 3 dim normal vector */
	3, GL_FLOAT, GL_FALSE, combinedBufferSize,
	/* offset within buffer is 3 (xyz pos) + 2 (uv) = 5*/
	(const GLvoid*) (5 * sizeof(GLfloat)));

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_renderBackend.ext_glBindVertexArray(0);

	auto m = Mesh(gVAO, meshData.getVertexCount());
	m_loadedMeshes[name] = m;

	logging::Info() << "Mesh with name '" << name << "' loaded";
	return m;
}
