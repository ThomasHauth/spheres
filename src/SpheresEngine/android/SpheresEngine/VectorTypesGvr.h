#pragma once

#include <SpheresEngine/VectorTypes.h>

inline glm::mat4 gvrToGlm( gvr::Mat4f gvrMatrix ) {
    glm::mat4 glmMatrix;

    for (size_t i = 0; i < 4; i++) {
        for (size_t k = 0; k < 4; k++){
            // glm is [colum][row] adressed while gvr matrix
            // uses the classical [row][colum] writing mode
            glmMatrix[i][k] = gvrMatrix.m[k][i];
        }
    }

    return glmMatrix;
}

inline glm::quat gvrToGlm( gvr_quatf gvr_q) {
    return glm::quat( gvr_q.qx,  gvr_q.qy, gvr_q.qz, gvr_q.qw);
}