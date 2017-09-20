#pragma once

#include <SpheresEngine/InputEngine/UserId.h>

#include <vector>

/**
 * An input which has been transformed into an action which
 * can be processed by an engine aspect. This class is derived from
 * and then created by the InputTransformer class
 */
class InputAction {
public:

	/**
	 * virtual dtor to support overriding
	 */
	virtual ~InputAction() = default;

	/**
	 * add a user which is related to this input action
	 */
	void addUser(UserId id) {
		m_users.push_back(id);
	}

	/**
	 * Sets the action as handled
	 */
	void setHandled() {
		m_handled = true;
	}

	/**
	 * Returns true if the input has been handled
	 */
	bool isHandled() const {
		return m_handled;
	}

private:
	/*
	 * the user or users which this action is related to
	 */
	std::vector<UserId> m_users;

	/*
	 * Store is the action has been handlend by any aspect
	 */
	bool m_handled = false;
};
