#pragma once

#include <unordered_map>

/**
 * Compact data structure to transfer a set of user data to a a shader program
 */
struct ShaderUserDataContainer {
	/**
	 * Floats to update. The key is the number of the parameter in the shader program
	 * an the value is the float to set
	 */
	std::unordered_map<char, float> Floats;
};
