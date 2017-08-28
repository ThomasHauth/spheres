#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SpheresEngine/Log.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <cstring>
#include <ostream>

std::vector<RendererVisualChange> ShaderBackend::checkReload() {
	std::vector<RendererVisualChange> changes;

	m_reloadCheck++;
	if (m_reloadCheck > m_reloadCheckInterval) {
		m_reloadCheck = 0;

		std::vector < std::string > changedFilesToRemove;
		auto changedFiles = m_re.getChangedFiles();

		// check for changes ...
		for (auto & sp : m_loadedPrograms) {
			bool reload = false;
			for (auto changedFile : changedFiles) {
				std::cout << "changedFile " << changedFile << std::endl;

				// this changed
				auto thisChanged = sp.second.usesFile(changedFile);
				if (thisChanged) {
					changedFilesToRemove.push_back(changedFile);
				}

				reload = reload || thisChanged;
			}

			if (reload) {
				// this one needs to reload !
				auto loaded = this->loadProgram(sp.second.getName(), true,
						false, true);

				if (!loaded.isValid()) {
					logging::Error()
							<< "Shader update skipped to due loading error";
				} else {
					logging::Info() << "Reloaded shared program "
							<< loaded.getName() << " due to file change";
				}
				changes.push_back(
						RendererVisualChange::createShaderProgramReloaded(
								loaded.getName(), loaded.getId()));

			}
		}

		// remove all the changed files from the change list which
		// have been handeled here
		std::for_each(changedFilesToRemove.begin(), changedFilesToRemove.end(),
				[&changedFiles] ( std::string fn) {changedFiles.erase(fn);});
	}

	return changes;
}

std::pair<std::string, Shader> ShaderBackend::loadShader(std::string name,
		Shader::Type type, bool noWatch) {
	const auto filename_source = m_re.loadShader(name, noWatch);
	auto shader_source = filename_source.second;
	logging::Info() << "Shader " << name << " loaded";

	if (m_prefixVersion.size() > 0) {
		shader_source = m_prefixVersion + "\n" + shader_source;
	}

	auto shader = compileShader(name, shader_source, type);
	logging::Info() << "Shader " << name << " compiled";

	return std::make_pair(filename_source.first, shader);
}

ShaderProgram ShaderBackend::loadProgram(std::string name, bool ignoreCache,
		bool dontStore, bool noWatch) {

// is this program cached and can just be returned ?
	auto it = m_loadedPrograms.find(name);
	if ((it != m_loadedPrograms.end()) && !ignoreCache)
		return it->second;

	auto itProgDef = m_programDefinitions.find(name);
	if (itProgDef == m_programDefinitions.end()) {
		logging::Fatal() << "Shader Program definition " << name
				<< " not registered";
	}

	std::vector<Shader> compiledShaders;
	std::vector < std::string > fileNames;
	for (auto shader : itProgDef->second) {
		auto filename_shader = loadShader(shader.first, shader.second, noWatch);

		if (!filename_shader.second.isValid()) {
			return ShaderProgram::createInvalid(
					filename_shader.second.getFailString());
		}

		//if (!noWatch)
		//	fileWatchers.push_back(watcher_shader.first);
		compiledShaders.push_back(filename_shader.second);
		fileNames.push_back(filename_shader.first);
	}

	auto linkedProg = linkProgram(compiledShaders);
	linkedProg.setName(name);
	std::for_each(fileNames.begin(), fileNames.end(),
			[&linkedProg] ( std::string fn ) {linkedProg.addShaderFile(fn);});

//std::for_each(fileWatchers.begin(), fileWatchers.end(),
//			[&linkedProg]( FileWatcherPtr & pt ) {linkedProg.addWatcher(pt);});

	if (!dontStore)
		m_loadedPrograms[name] = linkedProg;
	return linkedProg;
}

Shader ShaderBackend::compileShader(std::string name, std::string src,
		Shader::Type type) {

	GLint compile_ok = GL_FALSE;

	GLenum shaderType;

	if (Shader::Type::Fragment == type)
		shaderType = GL_FRAGMENT_SHADER;
	else if (Shader::Type::Vertex == type)
		shaderType = GL_VERTEX_SHADER;
	else {
		logging::Fatal() << "shader type not supported";
		// otherwise, compiler error
		shaderType = GL_VERTEX_SHADER;
	}

	GLuint fs = glCreateShader(shaderType);

	char * cstr = new char[src.length() + 1];
	std::strcpy(cstr, src.c_str());

	const char * cstrConst = cstr;

	glShaderSource(fs, 1, &cstrConst, NULL);
	delete[] cstr;

	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) {
		GLchar shaderLog[1024];
		GLsizei actualLength = 0;
		glGetShaderInfoLog(fs, 1023, &actualLength, shaderLog);

		logging::Error() << "Cannot compile shader '" << name
				<< "' due to error: " << shaderLog;
		return Shader::createInvalid(std::string(shaderLog));
	}

	logging::Info() << "Shader compiled successfully";
	return Shader(fs);

}

GLint ShaderProgram::getAttribLocation(std::string name) const {
	GLint attribute_coord2d = glGetAttribLocation(getId(), name.c_str());
	if (attribute_coord2d == -1) {
		logging::Fatal() << "Cannot find attribute " << name << " in program";
		return 0;
	}
	return attribute_coord2d;
}

GLint ShaderProgram::getUniformLocation(std::string name) const {
	GLint attribute_coord2d = glGetUniformLocation(getId(), name.c_str());
	if (attribute_coord2d == -1) {

		GLint uniformNumber;
		glGetProgramiv(getId(), GL_ACTIVE_UNIFORMS, &uniformNumber);

		std::vector < GLuint > uniformIndices;
		std::vector<char*> uniformNames;
		logging::Info() << "Got " << uniformNumber << " active uniforms";

		for (int i = 0; i < uniformNumber; i++) {
			char name[255];
			GLsizei stringLength;
			/*uniformIndices​.push_back(GLuint(i));
			 uniformNames​.push_back(nullptr);
			 */
			// todo: not supported on Android
			//glGetActiveUniformName(getId(), i, 255, &stringLength, &name[0]);
			//logging::Info() << "Active Uniform name " << name;
		}
		//glGetActiveUniformInd

		logging::Fatal() << "Cannot find uniform " << name << " in program";
		return 0;
	}
	return attribute_coord2d;
}

void ShaderProgram::setUniform(std::string name, GLint i) const {
	glUniform1i(getUniformLocation(name), i);
}

void ShaderProgram::setUniform(std::string name, glm::mat4 m,
		GLboolean transpose) const {
	glUniformMatrix4fv(getUniformLocation(name), 1, transpose,
			glm::value_ptr(m));
}

void ShaderProgram::setUniform(std::string name,
		std::array<float, 16> mat4) const {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, mat4.data());
}

void ShaderProgram::setUniform(std::string name, float v) const {
	glUniform1f(getUniformLocation(name), v);
}

void ShaderProgram::setUniform(std::string name, glm::vec3 v) const {
	glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
}

void ShaderProgram::setName(std::string name) {
	m_programName = name;
}

std::string ShaderProgram::getName() const {
	return m_programName;
}

void ShaderProgram::applyUserValues(ShaderUserDataContainer cont) const {
	for (auto & user_float : cont.Floats) {
		auto uniform_name = toVar(user_float.first);
		setUniform(uniform_name, user_float.second);
	}
}

/*void ShaderProgram::addWatcher(FileWatcherPtr fw) {
 m_watches.push_back(fw);
 }*/

ShaderProgram ShaderBackend::linkProgram(std::vector<Shader> shaders) {
	GLint link_ok = GL_FALSE;
	GLuint program = glCreateProgram();

	for (auto & shader : shaders) {
		glAttachShader(program, shader.getId());
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		logging::Fatal() << "Cannot link OpenGL program";
		return 0;
	}

	logging::Info() << "Shader program linked";
	return ShaderProgram(program);
}
