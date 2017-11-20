#pragma once

#include <SpheresEngine/Visuals/VisualDataExtract.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>

#include <memory>

/**
 * Base content shared by all render targets
 */
class TargetData {
public:
	/**
	 * viewing matrix of the camera to render for
	 */
	glm::mat4 CameraMatrix;

	/**
	 * projection matrix for the render perspective
	 */
	glm::mat4 ProjectionMatrix;

};

/**
 * Base class for all implementations of a Render Target
 */
class RenderTargetBase {
public:
	/**
	 * support dtor overwrite by inherited classes
	 */
	virtual ~RenderTargetBase() = default;

	/**
	 * Prepare the render pipeline for rendering to this target. This is
	 * called before the first render command for this target is executed.
	 */
	virtual TargetData beforeRenderToTarget(VisualDataExtractContainer &,
			std::vector<std::shared_ptr<RenderBackendDetails>>&) = 0;

	/**
	 * Finishing render pipeline for rendering to this target. This is
	 * called after the last render command for this target is executed.
	 */
	virtual void afterRenderToTarget(VisualDataExtractContainer &) = 0;

protected:

	/**
	 * Convenience function to find the BackendDetails required by a target
	 * and cast and return it. Returns a non-owning pointer which you are
	 * not allowed to store outside of the lifetime of dt
	 */
	template<class TSpecificBackendDetails>
	TSpecificBackendDetails * extractBackendDetails(
			std::vector<std::shared_ptr<RenderBackendDetails>> dt) {
		for (auto & shared_p : dt) {
			auto casted = dynamic_cast<TSpecificBackendDetails *>(shared_p.get());
			// was this cast successfully ?
			if (casted) {
				return casted;
			}
		}

		logging::Fatal()
				<< "Cannot find TSpecificBackendDetails in provided RenderBackendDetails";

		return nullptr;
	}
};
