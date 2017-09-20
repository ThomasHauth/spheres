#include <SpheresEngine/ResourceEngine/ResourceEngineAbstract.h>
#include "../RenderEngine/Texture.h"
#include <SpheresEngine/ResourceEngine/MeshLoader.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <algorithm>

void ResourceEngineAbstract::freeResources() {
	//for (TexturesMap::iterator it = m_textures.begin(); it != m_textures.end(); it++) {

	for (auto & tex : m_textures) {

		// overrides of 'freeTexture' in subclasses are not available in the destructor of 'ResourceEngineAbstract'
		// ... wow
		// but clear: the subclass has already been destroyed and therefore is invalid to be used
		this->freeTexture(tex.second.get());
	}
	m_textures.clear();

	for (auto & snd : m_sounds) {
		this->freeSound(snd.second);
		// don't delete sound pointer, it is either only a pointer ( SDL )
		// or a string ( Android )
	}
	m_sounds.clear();

	for (auto & snd : m_music) {
		this->freeMusic(snd.second);
		// don't delete music pointer, it is either only a pointer ( SDL )
		// or a string ( Android )
	}
	m_music.clear();
}

MeshData ResourceEngineAbstract::loadMesh(std::string meshName) {

	// not very optimal atm, as we will parse a file multipl times to extract
	// the one mesh we are looking for. In production, .obj parsing will anyhow
	// be replaced by a faster binary-based method
	MeshLoader ml;

	// todo: break up file name & mesh name
	std::vector < std::string > meshNamesSplit;
	boost::split(meshNamesSplit, meshName, boost::is_any_of("_"),
			boost::token_compress_on);

	if (meshNamesSplit.size() < 2) {
		logging::Fatal() << "Cannot split OBJ filename from mesh name "
				<< meshName;
	}
	const auto fileNamePart = meshNamesSplit[0];
	// only use the first part of the mesh name, blender generates a third object name part
	// from the mesh name, but we will ignore this here
	const auto meshNameOnly = meshNamesSplit[1];

	const std::string fileName(getMeshPrefix() + fileNamePart + ".obj");
	auto tf = loadTextFile(fileName);

	auto loadedMeshes = ml.loadMesh(tf);

	for (auto & kvMesh : loadedMeshes) {
		std::vector < std::string > namesSplit;
		boost::split(namesSplit, kvMesh.first, boost::is_any_of("_"),
				boost::token_compress_on);
		// only compare the first part of the mesh name and ignore the rest
		if (namesSplit.size() > 0) {
			if (namesSplit[0] == meshNameOnly) {
				return kvMesh.second;
			}
		}
	}
	logging::Fatal() << "Mesh with name " << meshName
			<< " was not found in file " << fileName;
	// keep the compiler happy !
	return MeshData();
}
