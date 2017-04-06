#pragma once

#include <SpheresEngine/RenderEngine/VisualRendererBase.h>

/**
 * This class takes care of setting up meshes and rendering them for every frame
 */
class MeshRenderer: public VisualRendererBase {
public:

	/*
	 * Ensures the texture and shader program for this mesh are properly loaded before
	 * rendering
	 * @return true if the provided visual was prepared by this renderer
	 */
	bool prepare(VisualAbstract &, RenderBackendBase &, ResourceEngine &)
			override;

	/**
	 * Render a set of meshes from the information contained in their visual extracts
	 */
	std::vector<RendererVisualChange> render(RenderBackendBase &,
			VisualDataExtractContainer const&, TargetData const&) override;
};
