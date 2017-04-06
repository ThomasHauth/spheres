#pragma once

#include <SpheresEngine/RenderEngine/OpenGLInclude.h>

#include <string>

/** class to communicate changes done by one renderer
 * to some visual (for example reload texture or shader) back to
 * the Visual classes in the logic thread
 */
class RendererVisualChange {
public:

	/**
	 * This enum contains the type of the change of thi class
	 */
	enum class EventTypeEnum {
		// shader program has been reloaded, the name of the
		// reloaded shader is in ShaderProgramName and the GLId in
		// ShaderProgramId
		ShaderProgramReload
	};

	/**
	 * Create a visual change which requests a shader reload, probably
	 * because the source code of a shader was changed on disk
	 */
	static RendererVisualChange createShaderProgramReloaded(std::string name,
			GLuint id) {
		RendererVisualChange rv;
		rv.EventType = EventTypeEnum::ShaderProgramReload;
		rv.ShaderProgramName = name;
		rv.ShaderProgramId = id;
		return rv;
	}

	/**
	 * Convenient string conversion for log output
	 */
	std::string toString() const {
		return "ShaderProgram " + ShaderProgramName + " reload";
	}

	/**
	 * Stores the type of change transferred by this class
	 */
	EventTypeEnum EventType;

	/**
	 * Name of the shader program which needs to be reloaded. Only set in
	 * case the EventType is  ShaderProgramReload
	 */
	std::string ShaderProgramName;

	/**
	 * OpenGL id of the shader program which needs to be reloaded. Only set in
	 * case the EventType is  ShaderProgramReload
	 */
	GLuint ShaderProgramId;
};

