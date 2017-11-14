#pragma once

#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>
#include <SpheresEngine/EntityEngine/Entity.h>
#include <SpheresEngine/EntityEngine/EntityList.h>
#include <SpheresEngine/Signals.h>
#include <SpheresEngine/Util.h>

// forward declare used data structures
struct VisualDataExtractContainer;
class RenderEngine;

/**
 * This engine manages the lifetime of entities. It makes sure the aspects of each
 * entity are executed each time step and ensures the visual updates are properly propagated
 * to entity visuals.
 * Furthermore, it extracts the visual data for rendering from all added entities.
 */
class EntityEngine final : boost::noncopyable {
public:

	/**
	 * Add a new entity to be managed by this engine
	 */
	void addEntity(uniq<Entity> ent,
			EntityList * managedList = nullptr) {
		if (managedList) {
			managedList->push_back(ent.get());
		}

		//todo: register with physics engine


		m_entities.push_back(std::move(ent));
	}

	/**
	 * Get the const list of all added entities
	 */
	EntityListUniq const& getEntities() const {
		return m_entities;
	}

	/**
	 * Get the non-const list of all added entities
	 */
	EntityListUniq & getEntities() {
		return m_entities;
	}

	/**
	 * execute one time step on this engine
	 * @param deltaTime the passed time since the last call to step()
	 */
	void step(float deltaTime);

	/**
	 * Propagate the prepared visuals to the entities which hold handles
	 * to visual ids
	 */
	void updatePreparedVisuals(RenderEngine::PrepareVisualList prepList);

	/**
	 * Update the data within one visual. This can be used to write back data
	 * changed on the render thread.
	 */
	void updateVisuals(std::vector<RendererVisualChange> const& prepList);

	/**
	 * Extract the render data of the visual of all added entities
	 */
	void extractVisualData(VisualDataExtractContainer & ex);

	/**
	 * This signal is called every simulation time step. This slot can be used
	 * by aspects who need to execute code on every time step.
	 */

	typedef slots::Slot<float> OnTimeStep_slot;

	OnTimeStep_slot OnTimeStep;

private:

	/**
	 * List of all entities added to this engine
	 */
	EntityListUniq m_entities;
};

