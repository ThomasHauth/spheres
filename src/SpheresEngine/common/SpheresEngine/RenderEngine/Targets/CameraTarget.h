#pragma once

#include "RenderTargetBase.h"
#include <SpheresEngine/Visuals/VisualDataExtract.h>

/**
 * Target which can be used to render the regular OpenGL 3d projection
 */
class CameraTarget final : public RenderTargetBase {
public:
	/**
	 * Called before any render command. Computes the camera & projection matrix
	 * and stores them in the target datat which is used by the RenderBackends
	 * and shaders
	 */
	TargetData beforeRenderToTarget(VisualDataExtractContainer & vd,
			std::vector<RenderBackendDetails*> backendDetails);

	/**
	 * Finish rendering to this target. Nothing done here (yet)
	 */
	void afterRenderToTarget(VisualDataExtractContainer & vd);
};
