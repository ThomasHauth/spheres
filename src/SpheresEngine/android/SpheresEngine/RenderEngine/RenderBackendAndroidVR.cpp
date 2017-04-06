#include "RenderBackendAndroidVR.h"
#include <SpheresEngine/RenderEngine/CommonOpenGL/GLSupport.h>
#include <SpheresEngine/VectorTypesGvr.h>
#include "RenderBackendAndroidVRDetails.h"

namespace {

static gvr::Sizei HalfPixelCount(const gvr::Sizei& in) {
	// Scale each dimension by sqrt(2)/2 ~= 7/10ths.
	gvr::Sizei out;
	out.width = (7 * in.width) / 10;
	out.height = (7 * in.height) / 10;
	return out;
}
}

ShaderBackend & RenderBackendAndroidVR::getShaderBackend() {
	return m_shaderBackend;
}

MeshBackend & RenderBackendAndroidVR::getMeshBackend() {
	return m_meshBackend;
}

void RenderBackendAndroidVR::ext_glGenVertexArrays(GLuint i, GLuint * vaoid) {
	glGenVertexArrays(i, vaoid);
}

void RenderBackendAndroidVR::ext_glBindVertexArray(GLuint vao) {
	glBindVertexArray(vao);
}

void RenderBackendAndroidVR::initRenderer() {

	// this must be called on the thread which also dispatches the OpenGL draw calls
	// later
	m_gvr_api->InitializeGl();

	// create viewport to render to
	m_scratch_viewport = std14::make_unique<gvr::BufferViewport>(
			m_gvr_api->CreateBufferViewport());
	// Because we are using 2X MSAA, we can render to half as many pixels and
	// achieve similar quality.
	m_render_size = HalfPixelCount(
			m_gvr_api->GetMaximumEffectiveRenderTargetSize());

	logging::Info() << "Creating SpawChain for render size "
			<< m_render_size.width << ":" << m_render_size.height;

	std::vector<gvr::BufferSpec> specs;

	// not used currently, can be usde for fixed HUD for example
	// create default buffer spects and modify to our needs
	specs.push_back(m_gvr_api->CreateBufferSpec());
	specs[0].SetColorFormat(GVR_COLOR_FORMAT_RGBA_8888);
	specs[0].SetDepthStencilFormat(GVR_DEPTH_STENCIL_FORMAT_DEPTH_16);
	specs[0].SetSize(m_render_size);
	specs[0].SetSamples(2);

	/*  for overlay

	 specs[1].SetSize({256, 128});
	 specs[1].SetColorFormat(GVR_COLOR_FORMAT_RGBA_8888);
	 specs[1].SetDepthStencilFormat(GVR_DEPTH_STENCIL_FORMAT_NONE);
	 specs[1].SetSamples(1);*/
	m_swap_chain = std14::make_unique<gvr::SwapChain>(
			m_gvr_api->CreateSwapChain(specs));

	m_viewport_list = std14::make_unique<gvr::BufferViewportList>(
			m_gvr_api->CreateEmptyBufferViewportList());

	logging::Info() << "Android VR Render Backend initialized";

	GL_CHECK_ERROR(glClearColor(1.0f, 0.0f, 0.0f, 1.0f));

	// enable depth buffer
	GL_CHECK_ERROR(glEnable(GL_DEPTH_TEST));
	GL_CHECK_ERROR(glDepthFunc(GL_LESS));

	GL_CHECK_ERROR(glEnable(GL_CULL_FACE));
	// counter-clockwise checking for backside checks in triangles
	GL_CHECK_ERROR(glFrontFace(GL_CCW));
}

void RenderBackendAndroidVR::PrepareFramebuffer() {
	// Because we are using 2X MSAA, we can render to half as many pixels and
	// achieve similar quality.
	gvr::Sizei recommended_size = HalfPixelCount(
			m_gvr_api->GetMaximumEffectiveRenderTargetSize());
	if (m_render_size.width != recommended_size.width
			|| m_render_size.height != recommended_size.height) {
		// We need to resize the framebuffer.
		m_swap_chain->ResizeBuffer(0, recommended_size);
		m_render_size = recommended_size;
	}
}

std::vector<RenderBackendDetails*> RenderBackendAndroidVR::beforeRender() {

	PrepareFramebuffer();

	// setup the default viewpors for left and right eye
	m_viewport_list->SetToRecommendedBufferViewports();

	// Bind to first buffer, which is used to render to the left and right eyei
	m_frame = m_swap_chain->AcquireFrame();

	const uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;

	// A client app does its rendering here.
	// todo: better timing estimate here
	gvr::ClockTimePoint target_time = gvr::GvrApi::GetTimePointNow();
	target_time.monotonic_system_time_nanos +=
	//100000;
			kPredictionTimeWithoutVsyncNanos;

	m_head_view = m_gvr_api->GetHeadSpaceFromStartSpaceRotation(target_time);
	const auto head_view_glm = gvrToGlm(m_head_view);

	const auto left_eye_matrix = gvrToGlm(
			m_gvr_api->GetEyeFromHeadMatrix(GVR_LEFT_EYE));
	const auto right_eye_matrix = gvrToGlm(
			m_gvr_api->GetEyeFromHeadMatrix(GVR_RIGHT_EYE));
	const auto left_eye_view = left_eye_matrix * head_view_glm;
	const auto right_eye_view = right_eye_matrix * head_view_glm;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
	// todo: this has to be moved to the VR Eye target later
	m_frame.BindBuffer(0);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);  // Dark background so text shows up.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_viewport_list->GetBufferViewport(0,  m_scratch_viewport.get());

	// and also draw for the second eye
//	DrawWorld(left_eye_view, scratch_viewport_);
	/*	m_viewport_list_->GetBufferViewport(1, &scratch_viewport_);

	 DrawWorld(right_eye_view, scratch_viewport_);
	 */

	GLSupport::checkGLError("RenderBackendAndroidVR::beforePresent");

	return {new RenderBackendAndroidVRDetails(
				m_gvr_api,
				m_viewport_list.get(),
				m_scratch_viewport.get(),
				{	left_eye_view,right_eye_view},
				m_render_size)};

}

void RenderBackendAndroidVR::present() {

	// this will later happen in the VR Eye Target
	m_frame.Unbind();
	m_frame.Submit(*m_viewport_list, m_head_view);
}

