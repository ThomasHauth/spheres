#include "MeshVisual.h"

#include <SpheresEngine/Visuals/VisualDataExtract.h>

void MeshVisual::extractData(VisualDataExtractContainer & cont,
		ExtractOffset const& eo) {
	auto data = getData();

	// apply the existing offset position of the Entity
	data.Center = eo.Position.toGlm();

	// combine rotations of the entity with this mesh
	data.Rotation = eo.Rotation;

	cont.MeshVisuals.push_back(data);
}
