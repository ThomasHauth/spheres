#include <SpheresEngine/EntityEngine/Entity.h>

#include <memory>

void Entity::addVisual(VisualAbstract * visual) {
	m_visuals.push_back(visual);
}

void Entity::addVisualPlaceholder(PrepareVisualId id) {
	m_visualsPrepare.push_back(id);
}

void Entity::updatePreparedVisuals(RenderEngine::PrepareVisualList const& vis) {
	if (m_visualsPrepare.size() == 0)
		return;

	std::vector<PrepareVisualId> addedIds;
	for (auto const& v : vis) {
		for (auto vp : m_visualsPrepare) {
			if (vp == v.first) {
				this->addVisual(v.second);
				addedIds.push_back(vp);
				logging::Info() << "Prepared visual of type "
						<< v.second->getType() << " added to Entity";
			}
		}

	}

	// remove all ids of visuals which have been added
	for (auto v : addedIds) {
		auto it = std::remove(m_visualsPrepare.begin(), m_visualsPrepare.end(),
				v);
		m_visualsPrepare.erase(it, m_visualsPrepare.end());
	}
}

void Entity::updateVisuals(std::vector<RendererVisualChange> const& vcs) {
	// propagate to children
	for (auto const& vc : vcs) {
		for (auto const& v : m_visuals) {
			v->update(vc);
		}
	}
}
