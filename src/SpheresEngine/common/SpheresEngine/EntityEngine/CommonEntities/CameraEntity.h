#pragma once

#include "PositionMixin.h"
#include <SpheresEngine/EntityEngine/Entity.h>
#include <SpheresEngine/VectorTypes.h>

/**
 * This entity can be used to present a world-camera.
 * Is final because the EntityWithAspects is already integrated here.
 * If you need to derive from this, create a CameraEntityBase
 */
class CameraEntity: public Entity, public PositionMixin {
public:
	virtual ~CameraEntity() = default;

	/**
	 * set the lookAt parameter of the camera
	 */
	void lookAt(Vector3 l) {
		m_lookAt = l;
	}

	/**
	 * Data extraction for the render targer
	 */
	virtual void extractData(VisualDataExtractContainer & cont) {
		cont.CameraData.LookAt = m_lookAt;
		cont.CameraData.Position = getPosition();
	}

private:
	/**
	 * Stores the position to look at
	 */
	Vector3 m_lookAt;
};

