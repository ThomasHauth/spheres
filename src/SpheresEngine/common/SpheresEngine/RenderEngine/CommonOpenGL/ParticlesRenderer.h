#pragma once

#include <SpheresEngine/RenderEngine/VisualRendererBase.h>

/**
 * This renderer prepares and renders complex particle systems using
 * a special shader
 */
class ParticlesRenderer: public VisualRendererBase {
public:

	/**
	 * Loads the required shader and downloads all the data of the particles
	 * to OpenGL
	 */
	bool prepare(VisualAbstract &, RenderBackendBase &, ResourceEngine &)
			override;

	/**
	 * re-download the particle data, if they have been updated, selects the
	 * correct shader and renders the particles
	 */
	std::vector<RendererVisualChange> render(RenderBackendBase &,
			VisualDataExtractContainer const&, TargetData const&) override;

private:
	/**
	 * the one vertex which will be drawn instanced for all particle systems
	 */
	util::ValidValue<GLuint> m_protoVertex;
};
