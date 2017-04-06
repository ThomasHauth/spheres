#pragma once

#include <SpheresEngine/Visuals/VisualDataExtract.h>

class RenderBackendBase;
class TargetData;
class ResourceEngine;
class RendererVisualChange;

#include <vector>

/**
 * Base class for all classes which implement a renderer. Each renderer takes
 * care of rendering one specific set of visuals, for example meshes or particle systems
 */
class VisualRendererBase {
public:

	/**
	 * virtual dtor to support inheritance
	 */
	virtual ~VisualRendererBase() {

	}

	/**
	 * To implement by inheriting class. Must return true for visual types which can be rendered
	 * by a specific renderer implementation
	 */
	template<class TVisual>
	bool supportsVisual() {
		return false;
	}

	/**
	 * returns true if the this render was able to
	 * integrate this visual item
	 */
	virtual bool prepare(VisualAbstract &, RenderBackendBase &,
			ResourceEngine &) {
		return false;
	}

	/**
	 * Renders all visuals within the visual data extract
	 */
	virtual std::vector<RendererVisualChange> render(RenderBackendBase &,
			VisualDataExtractContainer const&, TargetData const&) = 0;
};
