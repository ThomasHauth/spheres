#pragma once

#include <SpheresEngine/VectorTypes.h>

// for GLM_GTX_transform to be available
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

/**
 * A Mixin class which can be used when an entity needs to support collisions in the physics
 * engine.
 */
class CollisionMixin {
public:

	enum class ShapeSource {
		Box, Cylinder, Meshes, None
	};

	void setShapeSource(ShapeSource sr) {
		m_shapeSource = sr;
	}

	void setBoxSize(Vector3 s) {
		m_boxSize = s;
	}

	ShapeSource getShapeSource() const {
		return m_shapeSource;
	}

	Vector3 getBoxSize() const {
		return m_boxSize;
	}

private:
	/**
	 * Stores the position of this mixin
	 */
	Vector3 m_boxSize = Vector3::zero();

	ShapeSource m_shapeSource = ShapeSource::None;
};
