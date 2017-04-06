#pragma once

#include <glm/mat4x4.hpp> // glm::mat4
#include <SpheresEngine/ResourceEngine/ResourceEngine.h>
#include <SpheresEngine/RenderEngine/RendererVisualChange.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderUserData.h>

#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <list>

class ShaderBackend;

/**
 * Base class for all classes which contain a GLid to refernce
 * to the OpenGL object they represent
 */
class GLIDBase {
public:

	/**
	 * Create this class and store the id
	 */
	GLIDBase(GLuint id) :
			m_id(id) {

	}

	/** provide default contsructor so
	 * class is usable in a map
	 */
	GLIDBase() = default;

	/** provide copy-constructor for the
	 * class to be usable in a map
	 */
	GLIDBase(const GLIDBase &) = default;

	/**
	 * Return the id stored by this class
	 */
	GLuint getId() const {
		return m_id;
	}

	/**
	 * Set the id which this class holds
	 */
	void setId(GLuint id) {
		m_id = id;
	}

private:

	/**
	 * internal storage of the id
	 */
	GLuint m_id = 0;
};

/**
 * Class to load and hold a reference an OpenGL
 * shader program. This class also takes care of setting the
 * shader uniforms and attributes. Multiple Shader classes form
 * a ShaderProgram.
 */
class ShaderProgram: public GLIDBase {
public:
	/**
	 * Create a new shader program with an existing id
	 */
	ShaderProgram(GLuint id) :
			GLIDBase(id) {
	}

	/** provide default ctor for the
	 * class to be usable in a map
	 */
	ShaderProgram() = default;

	/** provide copy-constructor for the
	 * class to be usable in a map
	 */
	ShaderProgram(const ShaderProgram &) = default;

	/**
	 * Get the GL id for the location of the attribute with name
	 */
	GLint getAttribLocation(std::string name) const;

	/**
	 * Return the GL id for the location of the uniform with name
	 */
	GLint getUniformLocation(std::string name) const;

	/**
	 * Set the value of a uniform of type integer
	 */
	void setUniform(std::string name, GLint i) const;

	/**
	 * Set the value of a uniform of type matrix, using th glm::mat4
	 * class as input
	 */
	void setUniform(std::string name, glm::mat4,
			GLboolean transpose = false) const;

	/**
	 * Set the value of a uniform of type matrix, using the flat array here
	 * as input
	 */
	void setUniform(std::string name, std::array<float, 16> mat4) const;

	/**
	 * Set the value of a uniform of type float
	 */
	void setUniform(std::string name, float v) const;

	/**
	 * Set the name of this shader, this is only used for debugging
	 */
	void setName(std::string name);

	/**
	 * Get the name of this shader
	 */
	std::string getName() const;

	/**
	 * Add a file name which has been used to load the shader code. This is
	 * used for debugging and to check which shaders need to be reloaded
	 * when a file on disk changes
	 */
	void addShaderFile(std::string fn) {
		m_shaderFiles.push_back(fn);
	}

	/**
	 * Checks if a file is used by this shader
	 */
	bool usesFile(std::string fn) const {
		return std::find(m_shaderFiles.begin(), m_shaderFiles.end(), fn)
				!= m_shaderFiles.end();
	}

	/**
	 * Get all the file names used by this shader
	 */
	std::vector<std::string> getShaderFiles() const {
		return m_shaderFiles;
	}

	/**
	 * Return an invalid (not loaded) shader which serves as
	 * a place holder
	 */
	static ShaderProgram createInvalid(std::string failString) {
		ShaderProgram s;
		s.m_isValid = false;
		s.m_failString = failString;
		return s;
	}

	/**
	 * Update a list of user values within the shader
	 */
	void applyUserValues(ShaderUserDataContainer cont) const;

	/**
	 * Gets the string which explains why the shader failed to compile
	 */
	std::string getFailString() const {
		return m_failString;
	}

	/**
	 * Returns true if the shader was properly loaded
	 */
	bool isValid() const {
		return m_isValid;
	}

private:

	/**
	 * Error string which will be set if the shader could not be loaded or compiled
	 */
	std::string m_failString;

	/**
	 * Stores if the shader has been correctly loaded and initialized
	 */
	bool m_isValid = true;

	/**
	 * Convert the slot number of a shader to string
	 */
	std::string toVar(char slot) const {
		return "user" + boost::lexical_cast<std::string>(int(slot));
	}

	/**
	 * Name of the program this shader is part of
	 */
	std::string m_programName;

	/**
	 * File names this shader has been loaded from. This is used to reload the shader
	 * once a file changes on disk
	 */
	std::vector<std::string> m_shaderFiles;
};

/**
 * This class holds the GL id of one shader, where multiple form a shader program
 */
class Shader: public GLIDBase {
public:
	/**
	 * Defining the type of shader
	 */
	enum class Type {
		Fragment, Vertex
	};

	/**
	 * Default ctor to store unloaded shaders in maps etc
	 */
	Shader() :
			GLIDBase(0) {
	}

	/**
	 * Create a new shader object using the GL id
	 */
	Shader(GLuint id) :
			GLIDBase(id), m_isValid(true) {
	}

	/**
	 * Create an invalid instance of this class. This instance can
	 * be used as a placeholder and store the information string
	 * why creating the shader failed
	 */
	static Shader createInvalid(std::string failString) {
		Shader s;
		s.m_isValid = false;
		s.m_failString = failString;
		return s;
	}

	/**
	 * Returns true if the Shader has been correctly loaded
	 */
	bool isValid() const {
		return m_isValid;
	}

	/**
	 * Returns the string which contains information of why the
	 * shader failed to compile
	 */
	std::string getFailString() {
		return m_failString;
	}

private:
	/**
	 * Contains string why the shader failed to compile
	 */
	std::string m_failString;

	/**
	 * True if the shader has been correctly loaded and compiled
	 */
	bool m_isValid = false;
};

/**
 * This backend loads, compiles and manages all loaded shaders.
 */
class ShaderBackend {
public:

	/**
	 * Create the shader backend using the resource engine and taking a
	 * prefix string which can be inserted before every shader code before
	 * compilation
	 */
	ShaderBackend(ResourceEngine & re, std::string prefixVersion = "") :
			m_re(re), m_prefixVersion(prefixVersion) {
	}

	/**
	 * The Definition of a program content. First is the file name of the
	 * shader and the second is the type of the shader
	 */
	typedef std::pair<std::string, Shader::Type> ProgramDefinition;

	/**
	 * List of program definitions containing all the shader file name and
	 * their types which belong to one program
	 */
	typedef std::vector<ProgramDefinition> ProgramDefinitionList;

	/**
	 * Load a shader program (aka all shaders belonging to this program)
	 */
	ShaderProgram loadProgram(std::string name, bool ignoreCache = false,
			bool dontStore = false, bool noWatch = false);

	/**
	 * Load a single shader using its name and type
	 */
	std::pair<std::string, Shader> loadShader(std::string name,
			Shader::Type type, bool noWatch = false);

	/**
	 * Add program definition which allows to load a shader program (and all
	 * required shaders) via the programName
	 */
	void addProgramDefinition(std::string programName,
			ProgramDefinitionList def) {
		m_programDefinitions[programName] = def;
	}

	/**
	 * Clear all program definitions which have been added so far
	 */
	void clearProgramDefinition() {
		m_programDefinitions.clear();
	}

	/** This does not actually update the Shader (it is read only), but
	 * returns the GLuint of the reloaded shader
	 * checks if any of the currently loaded shader programs needs to
	 * be updated
	 */
	std::vector<RendererVisualChange> checkReload();

private:
	/**
	 * Compile one shader
	 * @param name Name of the shader
	 * @param src Mult-line source code of the shader
	 * @param type Shader type
	 */
	Shader compileShader(std::string name, std::string src, Shader::Type type);

	/**
	 * Link together a shader program from multiple loaded shaders
	 */
	ShaderProgram linkProgram(std::vector<Shader> shaders);

private:
	/**
	 * map of name and object of all shader programs
	 */
	std::unordered_map<std::string, ShaderProgram> m_loadedPrograms;

	/**
	 * name and ProgamDefinitions of all shader program definitions declared
	 */
	std::unordered_map<std::string, ProgramDefinitionList> m_programDefinitions;

	/**
	 * Reference to the resource engine which is nedded to load the shade source
	 * code
	 */
	ResourceEngine & m_re;

	/**
	 *O pen GL ES does not like a version definitions, OpenGL on
	 * Desktop needs it. This prefix string is automaticallly attached
	 * at the front of every shader source
	 */
	std::string m_prefixVersion;

	/**
	 * Configures often per call to checkReload() the check is actually performed. With
	 * m_reloadCheckInterval = 50 the file content is checked every 50th time. This takes
	 * some load off of the resource engine and file system
	 */
	static constexpr size_t m_reloadCheckInterval = 50;

	/**
	 * Counts how often checkReload() has been called since the last actual reload
	 */
	size_t m_reloadCheck = 0;
};
