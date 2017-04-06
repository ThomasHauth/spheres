#pragma once

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_types.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>

/**
 * Backend details for the VR render backend. Transfers varaious API pointers
 * from the android activity to the spheres internal renderers.
 */
class RenderBackendAndroidVRDetails: public RenderBackendDetails {
public:
	/**
	 * Support proper cleanup of inherited class
	 */
	virtual ~RenderBackendAndroidVRDetails() = default;

	/**
	 * Create the render details and store all the required Google VR API
	 * entry points.
	 */
	RenderBackendAndroidVRDetails(gvr::GvrApi * gvr_api,
			gvr::BufferViewportList *viewport_list,
			gvr::BufferViewport *scratch_viewport,
			std::array<glm::mat4, 2> left_right_eye_view,
			gvr::Sizei render_size) :
			m_gvrApi(gvr_api), m_viewport_list(viewport_list), m_scratch_viewport(
					scratch_viewport), m_left_right_eye_view(
					left_right_eye_view), m_render_size(render_size) {
	}

	/**
	 * Activates the render viewport of the left eye
	 */
	void activateViewportLeftEye(
			gvr::BufferViewport & to_fill_scratch_viewport) {
		m_viewport_list->GetBufferViewport(GVR_LEFT_EYE,
				&to_fill_scratch_viewport);
	}

	/**
	 * Activates the render viewport of the right eye
	 */
	void activateViewportRightEye(
			gvr::BufferViewport & to_fill_scratch_viewport) {
		m_viewport_list->GetBufferViewport(GVR_RIGHT_EYE,
				&to_fill_scratch_viewport);
	}

	/**
	 * Return the total render size of the output buffer
	 */
	gvr::Sizei getRenderSize() const {
		return m_render_size;
	}

	/**
	 * Return both view matrices, for the left and right eye
	 */
	std::array<glm::mat4, 2> getLeftRightEyeView() const {
		return m_left_right_eye_view;
	}

	/**
	 * Return the pointer to the Google VR API
	 */
	gvr::GvrApi * getGvrApi() {
		return m_gvrApi;
	}

private:
	/**
	 * Hold the pointer to the google vr api
	 */
	gvr::GvrApi * m_gvrApi;

	/**
	 * Holds the list of all render viewports
	 */
	gvr::BufferViewportList *m_viewport_list;

	/**
	 * Pointer to the scratch viewport for rendering
	 */
	gvr::BufferViewport *m_scratch_viewport;

	/**
	 * Total size of the render output
	 */
	gvr::Sizei m_render_size;

	/**
	 * View Matrices for the left and right eye
	 */
	std::array<glm::mat4, 2> m_left_right_eye_view;

};
