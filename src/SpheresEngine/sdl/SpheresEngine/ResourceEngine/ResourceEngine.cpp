#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>

#include <SDL2/SDL_image.h>
#include <SpheresEngine/ResourceEngine/ResourceEngine.h>
#include <SpheresEngine/RenderEngine/Texture.h>

std::pair<std::string, std::string> ResourceEngine::loadShader(
		std::string const& shaderName, bool noWatch) {
	const std::string fileName(getShaderPrefix() + shaderName + ".txt");
	return std::make_pair(fileName, loadTextFile(fileName, noWatch));
}

std::string ResourceEngine::loadTextFile(std::string const& fileName,
		bool noWatch) {
	std::ifstream fileIn(fileName);

	std::string buffer;
	std::stringstream bufferAll;
	const size_t maxBuffer = 1024;
	char inBuffer[maxBuffer];

	if (fileIn.is_open()) {

		while (fileIn.good()) {
			std::string line;
			std::getline(fileIn, line);
			bufferAll << line << std::endl;
		}
		fileIn.close();
	} else {
		logging::Fatal() << "Cannot open file " << fileName;
	}

	if (m_installFileWatcher && !noWatch) {
		installWatch(fileName);
	}
	return bufferAll.str();
}

void ResourceEngine::installWatch(std::string fileName) {

	// extract only the folder part of the path
	// all editors will not write into a file but create a tempory one
	// and do atomic renaming
	auto filepath = boost::filesystem::path(fileName);
	auto folder_only = filepath.parent_path().native();

	if (m_installedWatches.find(folder_only) == m_installedWatches.end()) {
		// no watch yet installed, do so
		logging::Info() << "Installing inotify watch for folder " << folder_only
				<< ", watching for file " << fileName;
		m_installedWatches[folder_only] = std::make_shared < INotifyFileWatcher
				> (folder_only, m_inotify);
	}
	m_watchedFiles.insert(filepath.native());
	logging::Info() << "Watching file " << filepath.native() << " now";
}

std::set<std::string> & ResourceEngine::getChangedFiles() {
	// update the list of changed files

	// does not actually wait, because we are in
	// non-blocking mode
	m_inotify.WaitForEvents();

	// check if there is some new event from INotify
	size_t count = m_inotify.GetEventCount();
	while (count > 0) {
		InotifyEvent event;
		bool got_event = m_inotify.GetEvent(&event);

		if (got_event) {
			auto changedPath = event.GetWatch()->GetPath();
			// we watch the folder where the file is located, because most editors
			// recreate files when they write stuff out
			if (event.GetMask() & IN_CLOSE_WRITE) {
				auto fullChangePath = boost::filesystem::path(changedPath)
						/ boost::filesystem::path(event.GetName());
				logging::Info() << "Path " << fullChangePath.native()
						<< " changed";

				// do we watch this file in specific
				if (m_watchedFiles.find(fullChangePath.native())
						!= m_watchedFiles.end()) {
					m_recentlyChangedFiles.insert(fullChangePath.native());
				}
			}
		}
		count--;
	}

	return m_recentlyChangedFiles;
}

MusicPtr ResourceEngine::loadMusic(std::string const&) {
	/*
	 // todo: also make generic
	 if (m_music.find(musicName) != m_music.end()) {
	 // already loaded this texture
	 //logging::Info() << "Returning cached sound " << musicName;
	 return MusicPtr(m_music.at(musicName));
	 }

	 MusicPtr sample;
	 std::string fullPath = getSoundPrefix() + musicName + defaultSoundExt();
	 sample = Mix_LoadMUS(fullPath.c_str());
	 if (!sample) {
	 logging::Fatal() << "cannot load sound " << musicName
	 << " from pull path " << fullPath;
	 // handle error
	 }
	 m_music[musicName] = MusicPtr(sample);
	 return sample;*/
	return nullptr;
}

SoundPtr ResourceEngine::loadSound(std::string const&) {

	/*	if (m_sounds.find(soundName) != m_sounds.end()) {
	 // already loaded this texture
	 //logging::Info() << "Returning cached sound " << soundName;
	 return SoundPtr(m_sounds.at(soundName));
	 }

	 Mix_Chunk *sample;
	 std::string fullPath = getSoundPrefix() + soundName + defaultSoundExt();
	 sample = Mix_LoadWAV(fullPath.c_str());
	 if (!sample) {
	 logging::Fatal() << "cannot load sound sound " << soundName
	 << " from full path " << fullPath;
	 // handle error
	 }
	 m_sounds[soundName] = SoundPtr(sample);
	 return sample;*/
	return nullptr;
}

TexturePtr ResourceEngine::loadImage(std::string const& imageName,
		unsigned int frames) {

	std::string imageNameExt = imageName + defaultImageExt();

	auto res = checkTextureCache(imageNameExt);
	if (res.first) {
		return res.second;
	}

//usleep(10000);
// new texture, load from storage
	logging::Info() << "Loading texture " << imageNameExt;

// todo: this surface is leaked somehow, we need to free it when the
// texture is killed ...
	const std::string fullPath(getImagePrefix() + imageNameExt);
	SDL_Surface * surface = IMG_Load(fullPath.c_str());

	if (surface != nullptr) {

		// Check that the image's width is a power of 2
		if ((surface->w & (surface->w - 1)) != 0) {
			logging::Fatal() << " image width is not a power of 2";
		}

		// Also check if the height is a power of 2
		if ((surface->h & (surface->h - 1)) != 0) {
			logging::Fatal() << " image height is not a power of 2";
		}
// get the number of channels in the SDL surface
		GLint nOfColors = surface->format->BytesPerPixel;
		GLenum texture_format;
		if (nOfColors == 4)	// contains an alpha channel
				{
			//logging::Info() << "Loaded texture has alpha channel";
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		} else if (nOfColors == 3)     // no alpha channel
				{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		} else {
			logging::Fatal()
					<< " the image is not truecolor..  this will probably break";
			// todo: exception
			return TexturePtr();
			// this error should not go unhandled
		}

		auto texPtr = std14::make_unique<Texture>(surface->h, surface->w,
				nOfColors, texture_format, surface->pixels, frames);
#ifdef USE_SDL
		texPtr->m_sdlSurface = surface;
#endif

#ifdef DESCENT_TEXTURE_NAME_DEBUG
		texPtr->setTextureName(imageNameExt);
#endif

		auto texRef = texPtr.get();
		m_textures[imageNameExt] = std::move(texPtr);

		return texRef;
	} else {
		logging::Fatal() << "Can't load texture from file " << imageNameExt
				<< " from path " << fullPath;

		return TexturePtr(nullptr);
	}

	return nullptr;
}

void ResourceEngine::freeTexture(TexturePtr) {
//SDL_FreeSurface(tex->m_sdlSurface);
}

void ResourceEngine::freeSound(SoundPtr) {
//Mix_FreeChunk(snd);
}

void ResourceEngine::freeMusic(MusicPtr) {
//Mix_FreeMusic(snd);
}

