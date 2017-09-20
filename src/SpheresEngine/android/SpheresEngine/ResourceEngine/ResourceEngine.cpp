#include <SpheresEngine/ResourceEngine/ResourceEngine.h>
#include <SpheresEngine/Log.h>
#include <SpheresEngine/RenderEngine/Texture.h>
#include <SpheresEngine/ResourceEngine/MeshLoader.h>

TexturePtr ResourceEngine::loadImage(std::string const& imageName,
		unsigned int frames) {
	std::string imageNameExt = imageName + defaultImageExt();

	return loadImageIntern(imageNameExt, frames, true);
}

std::pair<std::string, std::string> ResourceEngine::loadShader(
		std::string const& shaderName, bool noWatch) {
	const std::string fileName(getShaderPrefix() + shaderName + ".txt");
	return std::make_pair(fileName, loadTextFile(fileName));
}

TexturePtr ResourceEngine::loadImageIntern(std::string const& imageNameExt,
		unsigned int frames, bool useCache) {

	if (useCache) {
		auto res = checkTextureCache(imageNameExt);
		if (res.first) {
			return res.second;
		}
	}

	//now work on the java interfacec ;)
	int id = m_javaInterface->loadImage(imageNameExt);

	// create texture which is already bound ( was done on the android side )
	auto tex = std14::make_unique<Texture>(id, frames);
	TexturePtr texPtr = tex.get();
	m_textures[imageNameExt] = std::move(tex);
	logging::Info() << "Image " << imageNameExt << " loaded";
	return texPtr;
}

void ResourceEngine::freeAllTextures() {
	for (auto & tex : m_textures) {
		m_javaInterface->freeTexture(tex.second->getID());
	}
	m_textures.clear();
	logging::Info() << "All textures freed";
}

void ResourceEngine::reloadAllTextures() {
	for (auto & tex : m_textures) {
		std::string texName = tex.first;

		// internal, because the image extension has already been added !
		auto texPtr = loadImageIntern(texName, tex.second->getAnimationFrames(),
				false);
		// we just need the id
		logging::Info() << "Prev id : " << tex.second->getID() << " newId: "
				<< texPtr->getID();
		tex.second->setID(texPtr->getID());
		tex.second->setPixelPointer(texPtr->getPixelPointer());
	}
}

std::string ResourceEngine::loadTextFile(std::string const& fileName, bool watch) {
	// todo: ibest to implement text file loading also on the Java side to be consistent
	//return m_javaInterface->readTextFile(fileName);
	logging::Info() << "requesting JavaSide to load textfile " << fileName;
	return m_javaInterface->loadTextFile(fileName);
}
/*
MeshData ResourceEngine::loadMesh(std::string meshName) {
	MeshLoader ml;

	const std::string fileName(getMeshPrefix() + meshName + ".obj");
	auto tf = loadTextFile(fileName);

	return ml.loadMesh(tf);
}
*/
