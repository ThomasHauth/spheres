#pragma once

#include <boost/noncopyable.hpp>
#include <SpheresEngine/RenderEngine/Targets/RenderTargetBase.h>
#include <SpheresEngine/DataTypes/StaticVector.h>
#include <SpheresEngine/Performance/SectionTimer.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <SpheresEngine/RenderEngine/RenderBackendBase.h>
#include <SpheresEngine/RenderEngine/RendererVisualChange.h>
#include <SpheresEngine/RenderEngine/VisualRendererBase.h>
#include <SpheresEngine/Util.h>
#include <SpheresEngine/VectorTypes.h>
#include <SpheresEngine/Visuals/ParticleSystemVisual.h>
#include <SpheresEngine/Visuals/ParticleSystemVisualList.h>
#include <SpheresEngine/RenderEngine/Targets/RenderTargetBase.h>
#include <vector>
#include <memory>
#include <list>
#include <algorithm>

class ResourceEngine;

/**
 * the render engine is never allowed to delete any visuals
 * it can only remove them from its RenderLists. The entities holding the
 * visuals are respnosible for deleting the actual objects
 * template<class OpenGLInterface, class OpenGLRender, class OpenGLState>
 */
class RenderEngine: boost::noncopyable {
public:

	/**
	 * Typedef when transferring ownership of visual pointers
	 * todo: make owning
	 */
	typedef VisualAbstract * OwningVisualAbstract;

	/**
	 * List to transfer ownership to visuals and also provide the
	 * ids which they have while being prepared
	 */
	typedef std::vector<std::pair<PrepareVisualId, OwningVisualAbstract>> PrepareVisualList;

	/**
	 * Create the render engine
	 * @param renderBackend transfer the rander backend which is used to render out the
	 * visuals
	 * @param resourceEngine reference to the resource engine which is required by the
	 * render backend to load textures and shaders
	 */
	RenderEngine(own<RenderBackendBase> renderBackend,
			ResourceEngine & resourceEngine) :
			m_renderBackend(std::move(renderBackend)), m_resourceEngine(
					resourceEngine) {

	}

	/**
	 * Open the display of the output render device
	 */
	void openDisplay() {
		m_renderBackend->openDisplay();
	}

	/**
	 * Close the display of the output render device
	 */
	void closeDisplay() {
		m_renderBackend->closeDisplay();
	}

	/**
	 * Initialize the render backend
	 */
	void initRenderer() {
		m_renderBackend->initRenderer();
	}

	/**
	 * Close the renderer backend, can free all resources
	 * allocated by the render backend
	 */
	void closeRenderer() {
		m_renderBackend->closeRenderer();
	}

	/**
	 * Add one renderer, which can render one
	 * visual primitive. For example Mesh or ParticleSystem
	 */
	void addRenderer(own<VisualRendererBase> r) {
		m_renderers.push_back(std::move(r));
	}

	/**
	 * Add render target. Each render target required all
	 * or some visuals to be rendered via its respective
	 * renderers.
	 */
	void addTarget(own<RenderTargetBase> t) {
		m_targets.push_back(std::move(t));
	}

	/**
	 * Add a visual which will be prepared (aka texture load etc.) during the
	 * next exeution of the render loop. The visual id which is returned can later
	 * be used to identify he visual, once it has been loaded.
	 * this can actually be called before anything else is initialized in the render engine
	 * when this and popPreparedVisuals is called, make sure the RenderEngine is not rendering...
	 */
	PrepareVisualId addToPrepareVisual(OwningVisualAbstract v) {
		auto thisId = m_currentPrepareId++;

		m_toPrepareVisuals.push_back(std::make_pair(thisId, v));
		return thisId;
	}

	/**
	 * Return a list of visuals which have been prepared during the last iteration
	 * of the render loop
	 */
	PrepareVisualList popPreparedVisuals() {
		auto tmpPreparedVisuals = m_preparedVisuals;
		m_preparedVisuals.clear();
		return tmpPreparedVisuals;
	}

	/**
	 * Return a list of RendererVisualChange which were generated during the last
	 * render loop
	 */
	std::vector<RendererVisualChange> popVisualChange() {
		auto tmpChanges = m_visualChange;
		m_visualChange.clear();
		return tmpChanges;
	}

	/**
	 * Update the visual data which is used to generate the next frame. A local
	 * copy is created which makes the original available again to be
	 * modified while the render thread is still running.
	 */
	void updateVisualData(VisualDataExtractContainer & ext) {
		// create a local copy
		m_lastVisualData = ext;
	}

	/**
	 * Execute all visual renderers for all targets
	 */
	void render();

	/**
	 * Return a pointer to the used render backend for direct access, for example
	 * to register shaders
	 */
	RenderBackendBase * getRenderBackend() {
		return m_renderBackend.get();
	}

private:

	/**
	 * Use all renderes to try to prepare a visual
	 */
	void prepareVisual(VisualAbstract *);

	/**
	 * List fo the visual change which were generated during the
	 * last render loops
	 */
	std::vector<RendererVisualChange> m_visualChange;

	/**
	 * List of renderers which have been registered with the render engine
	 */
	std::vector<std::unique_ptr<VisualRendererBase>> m_renderers;

	/**
	 * render targets, for which all the renderers are executed
	 */
	std::vector<std::unique_ptr<RenderTargetBase>> m_targets;

	/**
	 * The render backend (OpenGL SDL, OpenGL Android, VR) which is used to
	 * run the actual rendering
	 */
	std::unique_ptr<RenderBackendBase> m_renderBackend;

	/**
	 * The visual data extract which contains all data to be rendered. This
	 * can be updated by the game logic
	 */
	VisualDataExtractContainer m_lastVisualData;

	/**
	 * List of visual which need to be prepared
	 */
	PrepareVisualList m_toPrepareVisuals;

	/**
	 * List of visuals which are prepared
	 */
	PrepareVisualList m_preparedVisuals;

	/**
	 * Reference to the resource engine. Mainly used to load textures
	 */
	ResourceEngine & m_resourceEngine;

	/**
	 * Counter for the next visual id to hand out while visuals are being prepared.
	 * This number will be handed to the user if one visual was added for preparation
	 * and will be increased to have a unique number next time.
	 * Wrap around will not be a problem as there are only a few ids in flight.
	 */
	PrepareVisualId m_currentPrepareId = 0;
};
