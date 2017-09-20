#pragma once

#include <SpheresEngine/JavaInterface.h>
#include <SpheresEngine/ResourceEngine/ResourceEngineAbstract.h>

#include <string>
#include <set>

/**
 * Resource engine implementation for android. Mostly forwards calls to the
 * JavaInterface class
 */
class ResourceEngine: public ResourceEngineAbstract {
public:

	/**
	 * Load texture from android resources
	 * todo: don't load the same image twice, but return a ref to the actually loaded
	 */
	virtual TexturePtr loadImage(std::string const& imageName,
			unsigned int frames = 1) override;

	/**
	 * not implemented, yet
	 */
	virtual SoundPtr loadSound(std::string const& soundName) override
	{
		return SoundPtr(soundName);
	}

	/**
	 * not implemented, yet
	 */
	virtual MusicPtr loadMusic(std::string const& musicName) override
	{
		return SoundPtr(musicName);
	}

	/**
	 * Load shader from android resources
	 */
	std::pair<std::string, std::string> loadShader(
			std::string const& shaderName, bool noWatch = false) override;

	/**
	 * Set the java interface class. This needs to be done before any
	 * call into the ResourceEngine
	 */
	void setJavaInterface(JavaInterfaceNonNative * ji) {
		m_javaInterface = ji;
	}

	/**
	 * Load mesh from android resources
	 */
	//MeshData loadMesh(std::string meshName) override;
	/**
	 * this is necessary if the application has been paused
	 */
	void reloadAllTextures();

	/**
	 * needs to be done before android destroys the OpenGL ES context
	 */
	void freeAllTextures();

protected:
	/**
	 * not implementet yet
	 */
	void freeTexture(TexturePtr) override {
	}

	/**
	 * not implementet yet
	 */
	void freeSound(SoundPtr) override {
	}

	/**
	 * not implementet yet
	 */
	void freeMusic(MusicPtr) override {
	}

	/**
	 * Generic method to load a textfile (shaders) for example
	 * from the android resources
	 */
	std::string loadTextFile(std::string const& fileName, bool noWatch = false)
			override;

private:

	/**
	 * Intern method to load image from android resources
	 */
	TexturePtr loadImageIntern(std::string const& imageNameExt,
			unsigned int frames, bool useCache = true);

	/**
	 * Pointer to JavaInterface class to forward most of the
	 * actual loading to the java API
	 */
	JavaInterfaceNonNative * m_javaInterface = nullptr;

};
