#pragma once

#include "ResourceEngineTesting.h"

#include <unordered_map>

/**
 * Concrete ResourceEngine implementation to be used for
 * testing purposes. Currently only supports adding and
 * returning of test shaders.
 */
class ResourceEngineTesting: public ResourceEngine {
public:

	/**
	 * do nothing
	 */
	TexturePtr loadImage(std::string const&, unsigned int /*frames = 1*/)
			override {
		logging::NotImplemented();
		return TexturePtr();
	}

	/**
	 * do nothing
	 */
	SoundPtr loadSound(std::string const&) override {
		logging::NotImplemented();
		return SoundPtr();
	}

	/**
	 * do nothing
	 */
	MusicPtr loadMusic(std::string const&) override {
		logging::NotImplemented();
		return MusicPtr();
	}

	/**
	 * return a shader which was installed before for test purposes
	 */
	std::pair<std::string, std::string> loadShader(
			std::string const& shaderName, bool/* noWatch = false*/) override {
		return std::make_pair(shaderName, m_testShaders[shaderName]);
	}

	/**
	 * do nothing
	 */
	MeshData loadMesh(std::string /*meshName*/) override {
		logging::NotImplemented();
		return MeshData();
	}

	/**
	 * store a test shader for later usage via loadShader()
	 */
	void addTestShader(std::string name, std::string source) {
		m_testShaders[name] = source;
	}

protected:
	/**
	 * do nothing
	 */
	void freeTexture(TexturePtr) override {
	}

	/**
	 * do nothing
	 */
	void freeSound(SoundPtr) override {
	}

	/**
	 * do nothing
	 */
	void freeMusic(MusicPtr) override {
	}

private:
	/**
	 * stores the installed shaders handed out for unit tests
	 */
	std::unordered_map<std::string, std::string> m_testShaders;

};

