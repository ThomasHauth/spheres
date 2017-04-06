#include <SpheresEngine/RenderEngine/Targets/CameraTarget.h>

#include <glm/gtc/matrix_transform.hpp>

TargetData CameraTarget::beforeRenderToTarget(VisualDataExtractContainer & vd,
		std::vector<RenderBackendDetails*>) {
	TargetData td;

	td.CameraMatrix = glm::lookAt(vd.CameraData.Position.toGlm(),
			vd.CameraData.LookAt.toGlm(), glm::vec3(0, 1, 0));

	// todo: depends on platform, aka android
	td.ProjectionMatrix = glm::perspective(glm::radians(50.0f),
			float(800.0 / 600.0),
			// near clipping plane of the camera
			0.1f,
			// far clipping plane
			100.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	return td;

}

void CameraTarget::afterRenderToTarget(VisualDataExtractContainer &) {

}

