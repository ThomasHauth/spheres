#pragma once

#include <SpheresEngine/EntityEngine/Entity.h>
#include <SpheresEngine/VectorTypes.h>

#include "PositionMixin.h"
#include "RotationMixin.h"

/**
 * This is a game entity which can be positioned and rotated in 3d space
 */
class PositionedEntity: public PositionMixin,
		public RotationMixIn,
		public Entity {
public:
	virtual ~PositionedEntity() = default;

	/**
	 * Extracts all the position data for rendering this entity and extracts
	 * all the visuals added to this entity
	 */
	virtual void extractData(VisualDataExtractContainer & cont) {
		ExtractOffset eo;

		// store the offset coming from the entity location and rotation
		eo.Position = getPosition();
		eo.Rotation = getRotation();

		for (auto & v : getVisuals()) {
			v->extractData(cont, eo);
		}
	}

};
