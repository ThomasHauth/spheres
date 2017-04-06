#pragma once 

#include <vector>
#include <boost/noncopyable.hpp>
#include <SpheresEngine/Visuals/VisualDataExtract.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>

#include <SpheresEngine/StateEngine/Aspect.h>

/**
 * The Entity class is the main way to structure game content. Visuals can be added
 * to Entities in order to render them visually and aspects can be added to entities
 * in order to implement game logic.
 */
class Entity: private boost::noncopyable {
public:

	virtual ~Entity() = default;

	/**
	 * vector to hold list of Visuals base class
	 */
	typedef std::vector<VisualAbstract *> VisualAbstractList;
	/**
	 * vector which contains IDs of visuals which are currently being prepared
	 * by the renderer
	 */
	typedef std::vector<PrepareVisualId> VisualPrepareList;

	/**
	 * Extract all the render data of this Entity and its
	 * Visuals
	 */
	virtual void extractData(VisualDataExtractContainer & cont) {
		ExtractOffset eo;
		for (auto & v : m_visuals) {
			v->extractData(cont, eo);
		}
	}

	/**
	 * Add a visual to this entity
	 */
	void addVisual(VisualAbstract * visual);

	/**
	 * Add a placeholder for an visual which is currently being loaded by the
	 * render system
	 */
	void addVisualPlaceholder(PrepareVisualId id);

	/**
	 * A list of visuals which have been prepared during the last cycle by the render
	 * engine and now be added to the Entity.
	 */
	void updatePreparedVisuals(RenderEngine::PrepareVisualList const&);

	/**
	 * Data contained in visuals has been updated and now this entity needs to update
	 * its saves visuals.
	 */
	void updateVisuals(std::vector<RendererVisualChange> const&);

	/**
	 * Return a pointer to the first visual stored by this entity
	 */
	template<class TVisualType>
	TVisualType * getFirstVisual() {
		if (getVisuals().size() == 0) {
			logging::Fatal() << "No visuals in this entity";
		}
		return static_cast<TVisualType *>(getVisuals()[0]);
	}

	/**
	 * Get the full list of visuals of this entity
	 */
	VisualAbstractList & getVisuals() {
		return m_visuals;
	}

	/**
	 * Add an aspect to this entity.
	 */
	void addAspect(uniq<AspectBase> aspect) {
		aspect->internal_init(this);

		m_aspects.push_back(std::move(aspect));
	}

	/**
	 * Return a list of all aspects added to this entity
	 */
	AspectBaseListUniq & getAspectsBase() {
		return m_aspects;
	}

private:

	/**
	 * List of all visuals which have been fully loaded
	 */
	VisualAbstractList m_visuals;

	/**
	 * The list of visuals which is still being loaded by the render
	 * engine and added later
	 */
	VisualPrepareList m_visualsPrepare;

	/**
	 * Aspects associated with this entity
	 */
	AspectBaseListUniq m_aspects;
};

