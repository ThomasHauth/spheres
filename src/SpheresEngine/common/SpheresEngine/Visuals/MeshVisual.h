#pragma once

#include "VisualBase.h"
#include "VisualDataBase.h"
#include "MeshVisualData.h"
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>

#include <glm/vec3.hpp> // glm::vec3

struct VisualDataExtractContainer;

/**
 * This visual represents a 3d mesh including the normals
 * and a texture
 */
class MeshVisual: public VisualBase<MeshVisualData> {
public:

	/**
	 * Create a mesh visual
	 * @param meshName the mesh used for rendering
	 * @param textureName the texture drawn on this mesh
	 */
	MeshVisual(std::string meshName, std::string textureName) :
			m_meshName(meshName), m_texture(textureName) {

	}

	/**
	 * return the visual type
	 */
	virtual std::string getType() const override {
		return "MeshVisual";
	}

	/**
	 * Extracts the render information of this mesh to be
	 * used in the render thread
	 */
	void extractData(VisualDataExtractContainer & cont, ExtractOffset const& eo)
			override;

	/**
	 * The name of the mesh, mostly useful for debugging
	 */
	std::string getMeshName() const {
		return m_meshName;
	}

	/**
	 * Return the texture used for this visual
	 */
	std::string getTextureName() const {
		return m_texture;
	}

private:
	/**
	 * holds the mesh name
	 */
	std::string m_meshName;

	/**
	 * holds the texture name
	 */
	std::string m_texture;

};
