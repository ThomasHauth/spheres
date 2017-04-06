#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/Pathfinding/Node.h>
#include <SpheresEngine/Performance/SectionTimer.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/Timing.h>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>

#include "Entity.h"

void EntityEngine::extractVisualData(VisualDataExtractContainer & ex) {
	for (auto & e : getEntities()) {
		e->extractData(ex);
	}
}

void EntityEngine::step(float deltaTime) {
	OnTimeStep.signal(deltaTime);
}

void EntityEngine::updateVisuals(
		std::vector<RendererVisualChange> const& prepList) {
	// an update does not happen often, mostly when files are changed
	for (auto & e : getEntities()) {
		e->updateVisuals(prepList);
	}
}

void EntityEngine::updatePreparedVisuals(
		RenderEngine::PrepareVisualList prepList) {
	// todo: could be optimized by having a list of visuals which still needs prepared visuals

	for (auto & e : getEntities()) {
		e->updatePreparedVisuals(prepList);
	}
}

