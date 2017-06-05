#pragma once

#include <SpheresEngine/Util.h>
#include <SpheresEngine/Engines.h>
#include <SpheresEngine/Signals.h>

#include <boost/noncopyable.hpp>
#include <vector>

class Entity;

/**
 * Aspect base class which is not templated in order to hold various templated
 * aspects in one common list.
 * All user-derived Aspect classes need to derived from the class Aspect
 */
class AspectBase: boost::noncopyable {
public:

	/**
	 *  so the entity class can call the internl_* implementations below
	 */
	friend Entity;

	/**
	 * provide dtor to support inheritance and clean up all
	 * subscriptions of this aspect
	 */
	virtual ~AspectBase() {
		for (auto & sub_pair : m_managedSubsciptions) {
			sub_pair.first->unsubscribe(sub_pair.second);
		}
	}

	void addManagedSubscription(slots::SlotBase * pSlot,
			slots::SlotBase::subscribed_id id) {
		m_managedSubsciptions.push_back(std::make_pair(pSlot, id));
	}

private:

	/**
	 * internal initialization called by the Entity
	 */
	virtual void internal_init(Engines &, Entity *) = 0;

	std::vector<std::pair<slots::SlotBase *, slots::SlotBase::subscribed_id>> m_managedSubsciptions;
};

/**
 * Templated for easier user-side interfacing
 * Aspect class gets the engines and stuff it needs from outside,
 * via it's own constructor
 */
template<class TEntity>
class Aspect: public AspectBase {
public:
	/**
	 * provide dtor to support inheritance
	 */
	virtual ~Aspect() = default;

	/**
	 * can be overwritten by user-code and can be used by the
	 * aspect to register to all kind of signals related to the entity
	 * or provided by game engines
	 */
	virtual void init(Engines &, TEntity *) {
	}

	/**
	 * can be overwritten by user-code to deregister subscribed signals
	 */
	virtual void cleanup(Engines &) {
	}

private:

	/**
	 * internal initialization called by the Entity
	 */
	void internal_init(Engines & engines, Entity * ent) override final {
		// upcasting here is possible because we know the upper class
		TEntity * fc = static_cast<TEntity *>(ent);

		this->init(engines, fc);
	}

};

/**
 * typedef for the aspect base class owning pointer
 */
using AspectBaseListUniq = std::vector<uniq<AspectBase>>;
