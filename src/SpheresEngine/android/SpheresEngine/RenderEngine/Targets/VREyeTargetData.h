#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>

#include <SpheresEngine/VectorTypes.h>

/**
 * This struct holds all information required to render one VR eye view
 */
struct VREyeTargetData {
	/**
	 * Position of the eye
	 */
	Vector3 Position;

	/**
	 * Direction the eye looks at
	 */
	Vector3 LookAt;

	/**
	 * The projection matrix used to render this eye
	 */
	glm::mat4 ProjectionMatrix;
};

