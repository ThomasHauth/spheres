#pragma once

#include <SpheresEngine/VectorTypes.h>

/**
 * A Mixin class which can be derived from when creating new entity classes.
 * This MixIn represents one rotation via quaternions.
 */
class RotationMixIn {
public:

	/**
	 * return the stored rotation
	 */
	Quaternion getRotation() const {
		return m_quat;
	}

	/**
	 * set the rotation stored in this Mixin
	 */
	void setRotation(Quaternion quat) {
		m_quat = quat;
	}

	/**
	 * Rotates an Entity around by an angle around a vector
	 * Previous rotations are preserved
	 */
	void rotate(Vector3 rotateAroundThisVector, float angleInRad) {
		auto dg = glm::degrees(angleInRad);
		m_quat = glm::rotate(m_quat, dg, rotateAroundThisVector.toGlm());
	}

	/**
	 * reset the rotation to unity
	 */
	void resetRotation() {
		m_quat = Quaternion(glm::vec3(0.0f, 0.0f, 0.0f));
	}

private:

	/**
	 * Store the rotation as quaternion
	 */
	Quaternion m_quat = Quaternion(glm::vec3(0.0f, 0.0f, 0.0f));
};
