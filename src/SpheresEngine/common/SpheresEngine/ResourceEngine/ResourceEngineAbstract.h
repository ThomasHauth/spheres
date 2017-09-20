#pragma once

#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <SpheresEngine/Log.h>
#include <SpheresEngine/RenderEngine/Texture.h>
#include <SpheresEngine/RenderEngine/TextureRef.h>
#include <SpheresEngine/ResourceEngine/MeshData.h>
#include <SpheresEngine/SoundEngine/SoundPtr.h>
#include <SpheresEngine/Util.h>

#include <set>
#include <utility>
#include <string>
#include <memory>
#include <unordered_map>

/**
 * Abstract version of the resource engine to load all kind of game date like
 * mash files, textures and sounds from disk and cache them. This class needs
 * to be inherited for every platform. However, this abstract version still
 * provides some common functionality.
 */
class ResourceEngineAbstract: private boost::noncopyable {
public:

	/**
	 * Return the default extension for sound files
	 */
	std::string defaultSoundExt() const {
		return ".ogg";
	}

	/**
	 * Return the default extension for image files
	 */
	std::string defaultImageExt() const {
		return ".png";
	}

	/**
	 * Return the default folder prefix for images
	 */
	std::string getImagePrefix() const {
		return "images/";
	}

	/**
	 * Return the default folder prefix for sound files
	 */
	std::string getSoundPrefix() const {
		return "sounds/";
	}

	/**
	 * Return the default path to the shader files
	 */
	std::string getShaderPrefix() const {
		return "shader/";
	}

	/**
	 * Return the default path to object meshes
	 */
	std::string getMeshPrefix() const {
		return "models/";
	}

	/**
	 * Provide virtual dtor to support inheritance
	 */
	virtual ~ResourceEngineAbstract() = default;

	/**
	 * Free all resources allocated by this resource manager
	 */
	void freeResources();

	/**
	 * Abstract method to load an image
	 * @param imageName is the name of the image to load, without path and extension
	 * @param frames the number of animation frames this image contains. If 1, the image
	 * does not contain an animation.
	 */
	virtual TexturePtr loadImage(std::string const& imageName,
			unsigned int frames = 1) = 0;

	/**
	 * Abstract method to load a sound file
	 * @param soundName is the name of the sound to load, without path and extension
	 */
	virtual SoundPtr loadSound(std::string const& soundName) = 0;

	/**
	 * Abstract method to load a music file
	 * @param musicName is the name of the music file to load, without path and extension
	 */
	virtual MusicPtr loadMusic(std::string const& musicName) = 0;

	/**
	 * Abstract method to load a shader file
	 * @param shaderName is the name of the shader file to load, without path and extension
	 * @param noWatch if true, the shader will be reloaded if the file changes no disc
	 */
	virtual std::pair<std::string, std::string> loadShader(
			std::string const& shaderName, bool noWatch = false) =0;

	/**
	 * Abstract method to load a mesh
	 * @param meshName is the name of the mesh to load, without path and extension. When using
	 * .obj files which can contain multiple meshes in one file, the following convention is used:
	 * <obj file name>_<mesh name in file>.
	 * In formats which only contain one mesh/file, the file can simply be named
	 * <obj file name>_<mesh name in file>.some_file_ext for the strings in game to stay the
	 * same
	 */
	virtual MeshData loadMesh(std::string meshName);

	virtual std::string loadTextFile(std::string const& fileName, bool noWatch =
			false) = 0;

	/**
	 * Return the list of files that has been modified since the last execution of this method.
	 * This can optionally implemented by inheriting classes
	 */
	virtual std::set<std::string> & getChangedFiles() {
		return m_emptyChangedFileSet;
	}

protected:

	/**
	 * This will always be empty and is only a member so
	 * the default implementation getChangedFiles() in this
	 * class can return a reference.
	 */
	std::set<std::string> m_emptyChangedFileSet;

	/** called by the constructor of this class to give child classes a chance
	 * to free the image, for example delete it from the grafix ram
	 */
	virtual void freeTexture(TexturePtr tex) = 0;

	/**
	 * unload a specific sound
	 */
	virtual void freeSound(SoundPtr snd) = 0;

	/**
	 * unload a specific music piece
	 */
	virtual void freeMusic(MusicPtr msc) = 0;

	/**
	 * Check whether an image is already in the cache
	 */
	std::pair<bool, Texture *> checkTextureCache(
			std::string const& imageName) const {
		if (m_textures.find(imageName) != m_textures.end()) {
			// already loaded this texture
			//logging::Info() << "Returning cached texture " << imageName;
			return std::make_pair(true, m_textures.at(imageName).get());
		} else {
			return std::make_pair(false, nullptr);
		}
	}

	/**
	 * hash map type to cache textures
	 */
	typedef std::unordered_map<std::string, uniq<Texture>> TexturesMap;

	/**
	 * This hash map caches loaded textures by their names
	 */
	TexturesMap m_textures;

	/**
	 * hash map type to cache sounds
	 * using no uniq here, because SoundPtr is a placeholder for either a pointer
	 * coming from the android graphics system or a std::string in the case of SDL
	 */
	typedef std::unordered_map<std::string, SoundPtr> SoundMap;

	/**
	 * This hash map caches loaded sounds
	 */
	SoundMap m_sounds;

	/**
	 * This hash map type is used to cache loaded music pieces
	 * using no uniq here, because MusicMap is a placeholder for either a pointer
	 * coming from the android sound system or a std::string in the case of SDL
	 */
	typedef std::unordered_map<std::string, MusicPtr> MusicMap;

	/**
	 * This hash map holds all loaded music pieces
	 */
	MusicMap m_music;
};

