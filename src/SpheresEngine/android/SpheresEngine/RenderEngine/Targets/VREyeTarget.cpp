#include <SpheresEngine/RenderEngine/Targets/VREyeTarget.h>
#include <SpheresEngine/RenderEngine/RenderBackendAndroidVRDetails.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/GLSupport.h>
#include <SpheresEngine/VectorTypesGvr.h>

#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <cmath>

std::array<float, 16> VREyeTarget::MatrixToGLArray(const gvr::Mat4f& matrix) {
    // Note that this performs a *tranpose* to a column-major matrix array, as
    // expected by GL.
    std::array<float, 16> result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[j * 4 + i] = matrix.m[i][j];
        }
    }
    return result;
}


TargetData VREyeTarget::beforeRenderToTarget(VisualDataExtractContainer & vd,  std::vector<RenderBackendDetails*> backendDetails) {
	TargetData td;


	auto * androidVrBackend = extractBackendDetails<RenderBackendAndroidVRDetails>( backendDetails);

	//logging::Info () << "VREyeTarget selecting eye " << m_eyeNumber;
    gvr::BufferViewport scratch_viewport = androidVrBackend->getGvrApi()->CreateBufferViewport();

	if (m_eyeNumber == GVR_LEFT_EYE) {
		androidVrBackend->activateViewportLeftEye(scratch_viewport);
	}
	else if (m_eyeNumber == GVR_RIGHT_EYE) {
		androidVrBackend->activateViewportRightEye(scratch_viewport);
	}
	else{
		logging::Fatal () << "Unsupported Eye number " << m_eyeNumber;
	}

    const auto srcFov = scratch_viewport.GetSourceUv();

    // use glViewport to set the space to render into
    const gvr::Recti pixel_rect =
            CalculatePixelSpaceRect(androidVrBackend->getRenderSize(),
                                    srcFov);

    glViewport(pixel_rect.left, pixel_rect.bottom,
               pixel_rect.right - pixel_rect.left,
               pixel_rect.top - pixel_rect.bottom);

    const float kZNear = 1.0f;
    const float kZFar = 100.0f;
    const auto perspective = PerspectiveMatrixFromView(srcFov, kZNear, kZFar, m_eyeNumber);

    // compute proper projection matrix for this eye
    // todo, convert matrix formats
    td.ProjectionMatrix = perspective;
/*            glm::perspective(glm::radians(50.0f),
                        float(800.0 / 600.0),
                        // near clipping plane of the camera
                        0.1f,
                        // far clipping plane
                       500.0f);
*/
    // offset by the total camera position,
    // lookAt of Camera data is ignored on purpose as this is VR and
    // the user decides where to look
    td.CameraMatrix = glm::translate( androidVrBackend->getLeftRightEyeView()[m_eyeNumber],
                                       vd.CameraData.Position.toGlm() );


/*
    logging::Info() << "Setting viewport x, y =(" << pixel_rect.left << "," << pixel_rect.bottom << ") width, height = ("
            << pixel_rect.right - pixel_rect.left << ","
            << pixel_rect.top - pixel_rect.bottom << ")";*/



    /*if ( m_eyeNumber == GVR_LEFT_EYE)
        glClearColor(0.0f, 0.1f, 1.0f, 1.0f);  // Dark background so text shows up.
    else*/
      // Dark background so text shows up.



    // that's all, the correct eye buffer has been selected and the draw calls can be executed

    GLSupport::checkGLError("VREyeTarget::beforeRenderToTarget");

    return td;
}

void VREyeTarget::afterRenderToTarget(VisualDataExtractContainer & vd) {

}


glm::mat4 VREyeTarget::PerspectiveMatrixFromView(const gvr::Rectf& fov, float z_near,
                                            float z_far, int eyeNumber) const {
    gvr::Mat4f result;
    const float x_left = -std::tan(fov.left * M_PI / 180.0f) * z_near;
    const float x_right = std::tan(fov.right * M_PI / 180.0f) * z_near;
    const float y_bottom = -std::tan(fov.bottom * M_PI / 180.0f) * z_near;
    const float y_top = std::tan(fov.top * M_PI / 180.0f) * z_near;
    const float zero = 0.0f;

    assert(x_left < x_right && y_bottom < y_top && z_near < z_far &&
           z_near > zero && z_far > zero);
    const float X = (2 * z_near) / (x_right - x_left);
    const float Y = (2 * z_near) / (y_top - y_bottom);
    const float A = (x_right + x_left) / (x_right - x_left);
    const float B = (y_top + y_bottom) / (y_top - y_bottom);
    const float C = (z_near + z_far) / (z_near - z_far);
    const float D = (2 * z_near * z_far) / (z_near - z_far);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
        }
    }
    /*
     * this does not work because the values for X & Y are too large (>200)
    result.m[0][0] = X;
    result.m[0][2] = A;
    result.m[1][1] = Y;
    result.m[1][2] = B;
    result.m[2][2] = C;
    result.m[2][3] = D;
    result.m[3][2] = -1;
*/
 //todo: this is a one to one copy of the values from the Java VR example,
    // fix this and use the compute from above

    result.m[0][0] = 1.033;

    result.m[0][2] = -0.2712; // this is switched for other eye ...
   if (eyeNumber == GVR_LEFT_EYE){
        result.m[0][2] = - result.m[0][2];
    }

    result.m[1][1] = 0.86245;
    result.m[1][2] = -0.031415;
    result.m[2][2] = -1.002002;
    result.m[2][3] = -0.2002002;
    result.m[3][2] = -1;

    return gvrToGlm(result);
}

  gvr::Recti VREyeTarget::CalculatePixelSpaceRect(const gvr::Sizei& texture_size,
                                          const gvr::Rectf& texture_rect) const  {
    const float width = static_cast<float>(texture_size.width);
    const float height = static_cast<float>(texture_size.height);
    const gvr::Rectf rect = ModulateRect(texture_rect, width, height);
    const gvr::Recti result = {
            static_cast<int>(rect.left), static_cast<int>(rect.right),
            static_cast<int>(rect.bottom), static_cast<int>(rect.top)};
    return result;
}

  gvr::Rectf VREyeTarget::ModulateRect(const gvr::Rectf& rect, float width,
                               float height) const {
    gvr::Rectf result = {rect.left * width, rect.right * width,
                         rect.bottom * height, rect.top * height};
    return result;
}