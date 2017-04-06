#include <SpheresEngine/RenderEngine/CommonOpenGL/GLSupport.h>
#include <SpheresEngine/RenderEngine/Texture.h>

// todo: this is pontenially dangerous if render engine runs on dedicated thread on Android ...
void Texture::ensureOpenGLBind() {
	if (!m_glId.isValid()) {
#ifdef SPHERES_USE_ANDROID_OPENGL
		logging::Fatal() << "Texture bound was called on Android-compile code."
		<< "Must not happen because texture binding to context happens on Java-side "
		<< "when loading the texture's image.";
#endif
		// only executed for x86 build
		// for the Android platform, the textures are bound on the java side

		logging::Info() << "Binding texture " << getTextureName()
				<< " to OpenGL";
		{
			GLuint glId;
			GL_CHECK_ERROR(glGenTextures(1, &glId));
			m_glId.setValue(glId);
		}

		// Bind the texture object
		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, m_glId.get()));

		// Set the texture's stretching properties
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// this will not use any kind of filtering to blur / anti-alise the textures
		// setting when the texture is farther away than it's original size
		GL_CHECK_ERROR(
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		// setting when the textre is closer than it's original size
		GL_CHECK_ERROR(
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		// Edit the texture object's image data using the information SDL_Surface gives us
		GL_CHECK_ERROR(
				glTexImage2D(GL_TEXTURE_2D, 0, m_numberOfColors, m_width, m_height, 0, m_textureFormat, GL_UNSIGNED_BYTE, m_pixelPointer));
	}
}
