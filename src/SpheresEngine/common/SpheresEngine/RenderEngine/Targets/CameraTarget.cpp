#include <SpheresEngine/RenderEngine/Targets/CameraTarget.h>

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

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
	if (m_storeScreenshot.isValid()) {
		int x = 0, y = 0;
		int w = 800;
		int h = 600;

		std::vector <unsigned char *> pixels (w * h * 4); // 4 bytes for RGBA
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

		SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(pixels.data(), w, h, 8 * 4,
				w * 4, 0, 0, 0, 0);
		SDL_SaveBMP(surf, m_storeScreenshot.get().c_str());
		SDL_FreeSurface(surf);
	}
}
