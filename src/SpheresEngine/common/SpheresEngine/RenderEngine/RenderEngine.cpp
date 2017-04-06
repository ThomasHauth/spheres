#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include "../Log.h"

#include <algorithm>

void RenderEngine::prepareVisual(VisualAbstract * va) {

	for (auto & r : m_renderers) {
		if (r->prepare(*va, *m_renderBackend.get(), m_resourceEngine)) {
			logging::Info() << "Visual of type " << va->getType()
					<< " prepared";
			return;
		}
	}
	logging::Fatal() << "No renderer which can prepare " << va->getType();
}

void RenderEngine::render() {

	// prepare all new visuals ...
	for (auto & visual : m_toPrepareVisuals) {
		this->prepareVisual(visual.second);
	}
	// all prepared, switch lists
	m_preparedVisuals = m_toPrepareVisuals;
	m_toPrepareVisuals.clear();

	// check which shaders to reload
	auto visualChanges = m_renderBackend->getShaderBackend().checkReload();
	// and send visual changes requests to the backend, will be updated and some point ...
	m_visualChange.insert(m_visualChange.end(), visualChanges.begin(),
			visualChanges.end());

	auto renderBackendDetails =
	m_renderBackend->beforeRender();

	// targets can be for example right or left eye etc.
	// the Visual renderers must decide when and how they want to render on a target
	for (auto & t : m_targets) {

		auto const targetData = t->beforeRenderToTarget(m_lastVisualData, renderBackendDetails);
		for (auto & r : m_renderers) {
			auto visualChange = r->render(*m_renderBackend.get(),
					m_lastVisualData, targetData);

			for (auto & vc : visualChange) {
				logging::Info() << "Got a visual change :" << vc.toString();
			}

			m_visualChange.insert(m_visualChange.end(), visualChange.begin(),
					visualChange.end());
		}
		t->afterRenderToTarget(m_lastVisualData);
	}

	m_renderBackend->present();

	for ( auto * ptr : renderBackendDetails) {
		delete ptr;
	}
}
