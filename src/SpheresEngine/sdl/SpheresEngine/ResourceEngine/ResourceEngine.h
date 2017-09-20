#pragma once

#include <SpheresEngine/ResourceEngine/ResourceEngineAbstract.h>

#include <inotify-cxx.h>

#include <unordered_map>
#include <string>
#include <set>

/**
 * Class to simplify file watching for easy auto-reloads of game content
 */
class INotifyFileWatcher {
public:
	/**
	 * Create a watcher for a path
	 * @param path the path the watcher is installed for
	 * @param inot reference to the inotify instance to use to setup the
	 * watcher
	 */
	INotifyFileWatcher(std::string path, Inotify & inot) {
		logging::Info() << "Installing watcher on file " << path;
		m_installedWatch = std::make_shared < InotifyWatch
				> (path, IN_CLOSE_WRITE);
		inot.Add(m_installedWatch.get());
	}

	/**
	 * Return the watched path as string
	 */
	std::string getWatchedPath() const {
		return m_installedWatch->GetPath();
	}

	/**
	 * Remove the installed watch of this class
	 */
	void remove(Inotify & inot) {
		if (m_installedWatch.get()) {
			inot.Remove(m_installedWatch.get());
		}
	}

private:

	/**
	 * Store the object which was returned when the watch was created
	 */
	std::shared_ptr<InotifyWatch> m_installedWatch;
};

/**
 * Resource engine for SDL platform. This engine uses mainly SDL and c++ STL means to
 * load images and other files from disk.
 */
class ResourceEngine: public ResourceEngineAbstract {
public:

	/**
	 * Create the resource engine
	 * @param installFileWatcher If true, file watchers will be installed for all
	 * files which were loaded
	 */
	ResourceEngine(bool installFileWatcher = true) :
			m_installFileWatcher(installFileWatcher) {
		m_inotify.SetNonBlock(true);
	}

	/**
	 * The testing Resource engine will derive from this class, so this needs
	 * to provide a virtual dtor
	 */
	virtual ~ResourceEngine() = default;

	/*
	 * Don't load the same image twice, but return a ref to the actually loaded
	 * Returns the pointer to the loaded image
	 */
	TexturePtr loadImage(std::string const& imageName, unsigned int frames = 1)
			override;

	/**
	 * Load a sound file from disk, not implemented yet
	 */
	SoundPtr loadSound(std::string const& soundName) override;

	/**
	 * Load a music file from disk, not implemented yet
	 */
	MusicPtr loadMusic(std::string const& musicName) override;

	/**
	 * Load a shader from a text file
	 */
	std::pair<std::string, std::string> loadShader(
			std::string const& shaderName, bool noWatch = false) override;

	/**
	 * Loads a mesh data from a text file
	 */
	//MeshData loadMesh(std::string meshName) override;
	/** returns the set of changed files, a consumer is allowed to remove
	 * a file from this set. This assumes that one consumer (like the ShaderBackend)
	 * the sole user of a file (which is true)
	 */
	std::set<std::string> & getChangedFiles() override;

	/**
	 * Load content from a text file
	 */
	std::string loadTextFile(std::string const& fileName, bool noWatch = false)
			override;

protected:

	/**
	 * Unloads texture
	 */
	void freeTexture(TexturePtr) override;

	/**
	 * Unloads sound
	 */
	void freeSound(SoundPtr) override;

	/**
	 * Unloads music piece
	 */
	void freeMusic(MusicPtr) override;

	/**
	 * Install file watch for a specific path
	 */
	void installWatch(std::string filePath);

	/**
	 * the names of the files which have been changed since the last check
	 */
	std::set<std::string> m_recentlyChangedFiles;

	/**
	 * If true, file watchers should be installed for every file loaded
	 */
	bool m_installFileWatcher;

	/**
	 * contains the full file pathes which are watched
	 */
	std::set<std::string> m_watchedFiles;

	/**
	 * hash map of all path which are watched and the class which holds the
	 * watch pointer.
	 */
	std::unordered_map<std::string, std::shared_ptr<INotifyFileWatcher> > m_installedWatches;

	/**
	 * Inotify instance which is used to install and remove file watchers using Linux' Inotify
	 */
	Inotify m_inotify;
};
