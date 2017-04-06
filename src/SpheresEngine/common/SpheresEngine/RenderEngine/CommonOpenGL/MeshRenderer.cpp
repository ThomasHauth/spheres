#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshRenderer.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshBackend.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>
#include <SpheresEngine/RenderEngine/Targets/RenderTargetBase.h>

bool MeshRenderer::prepare(VisualAbstract & vd, RenderBackendBase & backend,
		ResourceEngine & re) {
	VisualAbstract * pVd = &vd;

	auto pMeshVisual = dynamic_cast<MeshVisual*>(pVd);

	// not our responsibility
	if (!pMeshVisual)
		return false;

	// get the texture and the mesh, will be auto-loaded if not done yet
	auto tex = re.loadImage(pMeshVisual->getTextureName());
	tex->ensureOpenGLBind();

	auto shaderProg = backend.getShaderBackend().loadProgram("default");
	auto mesh = backend.getMeshBackend().loadMesh(pMeshVisual->getMeshName(),
			*tex, shaderProg);

	pMeshVisual->getData().VertexArrayObjectId = mesh.getVao();
	pMeshVisual->getData().Shader = shaderProg;
	pMeshVisual->getData().VertexCount = mesh.getVertexCount();
	pMeshVisual->getData().TextureId = tex->getID();

	// integrate !
	return true;
}

std::vector<RendererVisualChange> MeshRenderer::render(
		RenderBackendBase & backend, VisualDataExtractContainer const & c,
		TargetData const& td) {

	for (auto & m : c.MeshVisuals) {
		if (!m.Visible)
			continue;
		glUseProgram(m.Shader.getId());

		// set the texture unit to use explicitly, cause Android VR is also using textures
		// to draw on
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m.TextureId);

		m.Shader.setUniform("tex", 0);

		m.Shader.setUniform("projection", td.ProjectionMatrix);
		m.Shader.setUniform("camera", td.CameraMatrix);

		glm::mat4 model_mat = glm::translate(glm::mat4(), m.Center);
		model_mat = glm::mat4_cast(m.Rotation) * model_mat;
		m.Shader.setUniform("model", model_mat);

		backend.ext_glBindVertexArray(m.VertexArrayObjectId);

		// draw the VAO
		glDrawArrays(GL_TRIANGLES, 0, m.VertexCount);

		// unbind the VAO
		backend.ext_glBindVertexArray(0);
		glUseProgram(0);
	}

	// this renderer will never do any changes
	return std::vector<RendererVisualChange>();
}
