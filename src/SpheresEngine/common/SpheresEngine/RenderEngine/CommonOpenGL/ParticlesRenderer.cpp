#include "ParticlesRenderer.h"

#include <SpheresEngine/RenderEngine/RenderBackendBase.h>
#include <SpheresEngine/RenderEngine/Targets/RenderTargetBase.h>
#include <SpheresEngine/RenderEngine/RendererVisualChange.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>

bool ParticlesRenderer::prepare(VisualAbstract &vd,
		RenderBackendBase & renderBackend, ResourceEngine &) {
	VisualAbstract * pVd = &vd;

	auto pPartVisual = dynamic_cast<ParticleSystemVisual*>(pVd);

	// not our responsibility
	if (!pPartVisual)
		return false;

	// do we have our prototype already ?
	if (!m_protoVertex.isValid()) {
		// The VBO containing the 4 vertices of the particles.
		// Thanks to instancing, they will be shared by all particles.
		const float psize = 0.05f;

		static const GLfloat g_vertex_buffer_data[] = { -psize, -psize, 0.0f,
				psize, -psize, 0.0f, -psize, psize, 0.0f, psize, psize, 0.0f, };
		GLuint billboard_vertex_buffer;
		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
				g_vertex_buffer_data, GL_STATIC_DRAW);
		m_protoVertex.setValue(billboard_vertex_buffer);
	}

	auto shaderProg = renderBackend.getShaderBackend().loadProgram("particles");
	pPartVisual->getData().Shader = shaderProg;

	// The VBO containing the positions and sizes of the particles
	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER,
			pPartVisual->getParticleCount() * 4 * sizeof(GLfloat), NULL,
			GL_STREAM_DRAW);
	pPartVisual->getData().vertexPositionBuffer = particles_position_buffer;

	// The VBO containing the colors of the particles
	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER,
			pPartVisual->getParticleCount() * 4 * sizeof(GLubyte), NULL,
			GL_STREAM_DRAW);
	pPartVisual->getData().vertexColorBuffer = particles_color_buffer;

	logging::Info() << "Registered particle system with "
			<< pPartVisual->getParticleCount() << " particles";

	return true;
}

std::vector<RendererVisualChange> ParticlesRenderer::render(RenderBackendBase &,
		VisualDataExtractContainer const& c, TargetData const& td) {
	for (auto & particles : c.ParticleSystems) {
		glUseProgram(particles.Shader.getId());

		particles.Shader.setUniform("projection", td.ProjectionMatrix);
		particles.Shader.setUniform("camera", td.CameraMatrix);

		glm::mat4 model_mat = glm::translate(glm::mat4(), particles.Center);

		// todo: apply rotation

		particles.Shader.setUniform("model", model_mat);

		particles.Shader.applyUserValues(particles.UserData);
		//backend.ext_glBindVertexArray(m.VertexArrayObjectId);

		// dowload new position data
		// todo: don't do this every frame !?!

		GLsizei particleCount = particles.getParticleCount();

		// only download if the there actually was a CPU-side change to the particles
		if (particles.Updated) {
			glBindBuffer(GL_ARRAY_BUFFER, particles.vertexPositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, particleCount * 4 * sizeof(GLfloat),
			NULL,
			GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
			const auto posSizeBuffer = particles.getPosSizeBuffer();
			glBufferSubData(GL_ARRAY_BUFFER, 0,
					particleCount * sizeof(GLfloat) * 4, posSizeBuffer);

			glBindBuffer(GL_ARRAY_BUFFER, particles.vertexColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, particleCount * 4 * sizeof(GLubyte),
			NULL,
			GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
			const auto colorBuffer = particles.getColorBuffer();
			glBufferSubData(GL_ARRAY_BUFFER, 0,
					particleCount * sizeof(GLubyte) * 4, colorBuffer);
		}

		// set up the arttributes for shader execution
		const auto attribVert = particles.Shader.getAttribLocation("vert");
		glEnableVertexAttribArray(attribVert);
		glBindBuffer(GL_ARRAY_BUFFER, m_protoVertex.get());
		glVertexAttribPointer(attribVert, // attribute. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*) 0 // array buffer offset
				);

		// 2nd attribute buffer : positions of particles' centers
		const auto attribVertPos = particles.Shader.getAttribLocation(
				"vertInstanceCenter");
		glEnableVertexAttribArray(attribVertPos);
		glBindBuffer(GL_ARRAY_BUFFER, particles.vertexPositionBuffer);
		glVertexAttribPointer(attribVertPos, // attribute. No particular reason for 1, but must match the layout in the shader.
				4, // size : x + y + z + size => 4
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*) 0 // array buffer offset
				);

		// 3rd attribute buffer : particles' colors
		const auto attribVertColor = particles.Shader.getAttribLocation(
				"vertInstanceColor");
		glEnableVertexAttribArray(attribVertColor);
		glBindBuffer(GL_ARRAY_BUFFER, particles.vertexColorBuffer);
		glVertexAttribPointer(2, // attribute. No particular reason for 1, but must match the layout in the shader.
				4, // size : r + g + b + a => 4
				GL_UNSIGNED_BYTE, // type
				GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
				0, // stride
				(void*) 0 // array buffer offset
				);

		glVertexAttribDivisor(attribVert, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(attribVertPos, 1); // positions : one per quad (its center) -> 1
		glVertexAttribDivisor(attribVertColor, 1); // positions : one per quad (its center) -> 1

		// ! todo: draw the array which is supplied within particles
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
		// draw the VAO
		//glDrawArrays(GL_TRIANGLES, 0, m.VertexCount);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// unbind the VAO
		//backend.ext_glBindVertexArray(0);
		glUseProgram(0);
	}

	return std::vector<RendererVisualChange>();
}

