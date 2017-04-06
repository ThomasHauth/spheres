#pragma once

#include <SpheresEngine/Visuals/VisualDataExtract.h>
#include <SpheresEngine/RenderEngine/Targets/RenderTargetBase.h>

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_types.h>

/**
 * Render target for one of the eyes of a Google VR system
 */
class VREyeTarget final : public RenderTargetBase {
public:

	/**
	 * Create the VR render target
	 * @param eyeNumber The eye number of this render target. Can either
	 * be GVR_LEFT_EYE or GVR_RIGHT_EYE
	 */
	VREyeTarget(int eyeNumber) :
			m_eyeNumber(eyeNumber) {
	}

	/**
	 * Compute the camera and projection matrix before running
	 * the renderers
	 */
	TargetData beforeRenderToTarget(VisualDataExtractContainer & vd,
			std::vector<RenderBackendDetails*> backendDetails);

	/**
	 * do nothing atm.
	 */
	void afterRenderToTarget(VisualDataExtractContainer & vd);

	/**
	 * Convert Google VR's matrix format to the one of glm
	 */
	std::array<float, 16> MatrixToGLArray(const gvr::Mat4f& matrix);

private:

	/**
	 * compute the rectangle size in pixel which is rendered to
	 */
	gvr::Recti CalculatePixelSpaceRect(const gvr::Sizei& texture_size,
			const gvr::Rectf& texture_rect) const;

	/**
	 * scale the size of a rectangle by a new width and height
	 */
	gvr::Rectf ModulateRect(const gvr::Rectf& rect, float width,
			float height) const;

	/**
	 * compute the perspective wiew
	 */
	glm::mat4 PerspectiveMatrixFromView(const gvr::Rectf& fov, float z_near,
			float z_far, int eyeNumber) const;

public:

	/**
	 * The number of the eye this target is rendering to. Can be GVR_LEFT_EYE
	 * or GVR_RIGHT_EYE
	 */
	int m_eyeNumber = -1;

};
