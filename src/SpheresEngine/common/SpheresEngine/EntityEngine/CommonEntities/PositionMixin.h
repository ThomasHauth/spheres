#pragma once

#include <SpheresEngine/VectorTypes.h>

#include <glm/gtx/rotate_vector.hpp>

/**
 * A Mixin class which can be derived from when creating new entity classes. It stores
 * position and provides related convenience methods.
 */
class PositionMixin {
public:

	/**
	 * return the stored position
	 */
	Vector3 getPosition() const {
		return m_position;
	}

	/**
	 * set the position stored in this Mixin
	 */
	void setPosition(Vector3 pos) {
		m_position = pos;
	}

	/**
	 * Relatively move the position
	 */
	void move(Vector3 movDelta) {
		m_position += movDelta;
	}

	/**
	 * Will compute the rotation position around a rotation center and completely
	 * reset the of position of this entity
	 */
	void rotateAround(Vector3 rotationCenter, Vector3 intialRotationPosition,
			Vector3 rotateAroundThisVector, float angleInRad) {

		m_position = rotationCenter;
		rotateAroundThisVector.normalize();

		auto rotatedVec = glm::rotate(intialRotationPosition.toGlm(),
				glm::degrees(angleInRad),
				glm::normalize(rotateAroundThisVector.toGlm()));

		m_position.setX(m_position.x() + rotatedVec.x);
		m_position.setY(m_position.y() + rotatedVec.y);
		m_position.setZ(m_position.z() + rotatedVec.z);
	}

private:
	/**
	 * Stores the position of this mixin
	 */
	Vector3 m_position = Vector3::zero();
};
