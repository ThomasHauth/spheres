#pragma once

#include <SpheresEngine/VectorTypes.h>
#include <SpheresEngine/Log.h>

#include <SpheresEngine/RenderEngine/OpenGLInclude.h>
#include <vector>

/**
 * This class holds the Position, UV texture and normal data of one
 * loaded mesh
 */
class MeshData {
public:
	/**
	 * The x,y,z position of each mesh vertex
	 */
	std::vector<Vector3> Position;

	/**
	 * The u and v texture position for each vertex
	 */
	std::vector<Vector2> UV;

	/**
	 * The normal vector for each vertex
	 */
	std::vector<Vector3> Normal;

	/**
	 * return the number of vertices in this mesh
	 */
	size_t getVertexCount() const {
		return Position.size();
	}

	/**
	 * Return one OpenGL-compatible version of the array packed with XYZ and UV
	 * coordinates
	 */
	std::vector<GLfloat> asXYZUV() const {
		return asConfigurable(true, true, false);
	}

	/**
	 * Return one OpenGL-compatible version of the array packed with XYZ and UV
	 * coordinates and 3d normals
	 */
	std::vector<GLfloat> asXYZUVNormals() const {
		return asConfigurable(true, true, true);
	}

	/**
	 * This method will create entries for each face of the mesh, with x y z u v
	 * which can be directly downloaded to OpenGL
	 */
	std::vector<GLfloat> asConfigurable(bool withXYZ, bool withUV,
			bool withNormal) const {
		std::vector<GLfloat> v;

		if ((UV.size() == 0) && (Position.size() > 0)) {
			logging::Fatal()
					<< "No UV information available to create OpenGL-compatible buffer";
		}

		for (size_t i = 0; i < Position.size(); i++) {

			if (withXYZ) {
				v.push_back(Position[i].x());
				v.push_back(Position[i].y());
				v.push_back(Position[i].z());
			}
			if (withUV) {

				v.push_back(UV[i].x());
				v.push_back(UV[i].y());
			}
			if (withNormal) {
				v.push_back(Normal[i].x());
				v.push_back(Normal[i].y());
				v.push_back(Normal[i].z());
			}
		}

		return v;
	}
};
