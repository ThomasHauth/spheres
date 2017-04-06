#pragma once

/**
 * Identifies one specific user if there are multiple users
 * sending input to the engine
 */
class UserId {
public:

	/**
	 * return the default user id which can be used in case there is only one
	 * user supported by the game implementation
	 */
	static UserId useDefault() {
		return UserId();
	}
};
