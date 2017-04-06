#pragma once

#include <SpheresEngine/VectorTypes.h>
#include <SpheresEngine/Visuals/MeshVisual.h>
#include <SpheresEngine/Visuals/ParticleSystemVisual.h>
#include <SpheresEngine/RenderEngine/Targets/CameraTargetData.h>
#include <vector>

/**
 * Container which brings together all the visual data to pass it
 * to the render engine
 */
struct VisualDataExtractContainer {

	/**
	 * Data used by the camera target
	 */
	CameraTargetData CameraData;

	/**
	 * All mesh data to render
	 */
	std::vector<MeshVisual::ExtractData> MeshVisuals;

	/**
	 * All particle system visuals data to render
	 */
	std::vector<ParticleSystemVisual::ExtractData> ParticleSystems;

	/**
	 * removes all content in this container
	 */
	void clear() {
		MeshVisuals.clear();
		ParticleSystems.clear();
	}
};

