#pragma once

#include <memory>
#include <string>
#include <boost/noncopyable.hpp>
#ifdef USE_SDL
#include <SDL2/SDL_surface.h>
#endif

#include <SpheresEngine/Log.h>
#include <SpheresEngine/Util.h>
#include <SpheresEngine/RenderEngine/OpenGLInclude.h>

#define DESCENT_TEXTURE_NAME_DEBUG 1

/**
 * Class to wrap the internals of keeping the id to a
 * loaded texture
 */
class Texture final: public boost::noncopyable {
public:

	/**
	 *  unbound texture, will be done here, this means the pixel data
	 *  has been loaded from disk but not downloaded to OpenGL
	 */
	Texture(unsigned int height, unsigned int width, GLint numberOfColors,
			GLenum textureFormat, void * pixelPointer, unsigned int animFrames =
					1) :
			m_animationFrames(animFrames), m_height(height), m_width(width), m_numberOfColors(
					numberOfColors), m_textureFormat(textureFormat), m_pixelPointer(
					pixelPointer) {

	}

	/**
	 * create Texture which has alrdeay been downloadad to OpenGL and can therefore
	 * be already referenced only by its OpenGL id
	 */
	Texture(GLuint id, size_t frameCount = 1) :
			m_animationFrames(frameCount), m_height(0), m_width(0), m_numberOfColors(
					0), m_textureFormat(0), m_pixelPointer(nullptr), m_glId(id) {

	}

	/**
	 * Get the OpenGL Id of the texture
	 */
	GLuint getID() const {
		if (!m_glId.isValid()) {
			logging::Fatal() << "Texture ID not set yet";
		}
		return m_glId.get();
	}

	/**
	 * Set the OpenGL Id of the texture. Might be needed if textures
	 * need to be reloaded by OpenGL if a mobile app was suspended
	 */
	void setID(GLuint id) {
		m_glId = id;
	}

	/**
	 * Set the pixel pointer of the texture. Might be needed if textures
	 * need to be reloaded by OpenGL if a mobile app was suspended
	 */
	void setPixelPointer(void * pixelPointer) {
		m_pixelPointer = pixelPointer;
	}

	/**
	 * Return the raw pixel pointer
	 */
	void * getPixelPointer() const {
		return m_pixelPointer;
	}

	/**
	 * Returns true if this texture contains more than one frame
	 * and therefore is animated
	 */
	bool isAnimated() const {
		return (m_animationFrames > 1);
	}

	/**
	 * Returns true if the texture has been bound in OpenGL
	 */
	bool isBound() const {
		return m_glId.isValid();
	}

	/**
	 * Returns the number of animation frames in this texture
	 */
	unsigned int getAnimationFrames() const {
		return m_animationFrames;
	}

	/**
	 * Download the texture ot OpenGL
	 * This has to be done here because we have to ensure, that
	 * all OpenGl calls happen on the Render Thread
	 */
	void ensureOpenGLBind();

#ifdef DESCENT_TEXTURE_NAME_DEBUG
	/**
	 * Set the name of the texture for easier debugging
	 */
	void setTextureName(std::string const& name) {
		m_textureName = name;
	}

	/**
	 * Return the name of the texture for easier debugging
	 */
	std::string const& getTextureName() const {
		return m_textureName;
	}
#endif

#ifdef USE_SDL
	/**
	 * Pointer to the SDL surface holding this texture. This is kept to be able
	 * to free the texture later on.
	 */
	SDL_Surface* m_sdlSurface = nullptr;
#endif

private:
#ifdef DESCENT_TEXTURE_NAME_DEBUG

	std::string m_textureName;
#endif

	/*
	 * Number of frames if this texture is used as an animation
	 * If just a regular texture, this is 1
	 */
	unsigned int m_animationFrames;

	/**
	 * Height in pixels
	 */
	unsigned int m_height;

	/**
	 * Width in pixels
	 */
	unsigned int m_width;

	/**
	 * Number of colors in this texture. 4 means there
	 * is an alpha channel
	 */
	GLint m_numberOfColors;

	/**
	 * OpenGL texture format, for example GL_RGBA
	 */
	GLenum m_textureFormat;

	/**
	 * pointer to the pixel data of this texture
	 */
	void * m_pixelPointer;

	/**
	 * OpenGL id of the texture, once it was successfully bound
	 */
	util::ValidValue<GLuint> m_glId;

};
