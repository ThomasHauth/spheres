#pragma once

#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderUserData.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

/**
 * Base class for all the Visual data classes
 */
struct VisualDataBase {

	/**
	 * If true, this visual should be rendered
	 */
	bool Visible = true;
};

/**
 * Mixin struct for visuals which have a 3d location in the
 * game world
 */
struct LocatedVisualDataBase {
	/**
	 * Center position of the visual in the game world
	 */
	glm::vec3 Center;

	/**
	 * Rotation of the visual
	 */
	glm::quat Rotation;
};

/**
 * Mixin struct for visuals using a shader to render
 */
struct ShadedVisualDataBase {

	/**
	 * Shader Program used for rendering this visual
	 */
	ShaderProgram Shader;

	/**
	 * User data which is passed to the shader for rendering
	 * specifically this visual
	 */
	ShaderUserDataContainer UserData;
};
