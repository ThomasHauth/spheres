#pragma once

#include <boost/lexical_cast.hpp>
#include <stddef.h>
#include <SpheresEngine/ResourceEngine/MeshData.h>
#include <SpheresEngine/Util.h>
#include <vector>
#include <string>

/**
 * Class which can load a mesh from OBJ files
 */
class MeshLoader {
public:

	/**
	 * Load and return mesh data from a multi-line string
	 * loaded from the mesh .obj file
	 */
	MeshData loadMesh(std::string meshData);

	/**
	 * This tuple contains all the (optional) data that can be extracted
	 * from one OBJ file line. The order of the tuple:
	 * index of the position, texture pos and normal pos
	 */
	typedef std::tuple<util::ValidValue<int>, util::ValidValue<int>,
			util::ValidValue<int>> PosTexNormalTuple;

	/**
	 * Parses one line of the OBJ file
	 * public for better test-ability
	 */
	PosTexNormalTuple parseFaceTuple(std::string ft) const;

private:

	/**
	 * Utility function to parse a vector of strings into a three-dimensional
	 * vector class
	 */
	template<class VectorType>
	VectorType parseVector(std::vector<std::string> strs,
			size_t offset = 1) const {

		assert(strs.size() >= offset + 3);
		return VectorType(
				boost::lexical_cast<typename VectorType::value_type>(
						strs[offset + 0]),
				boost::lexical_cast<typename VectorType::value_type>(
						strs[offset + 1]),
				boost::lexical_cast<typename VectorType::value_type>(
						strs[offset + 2]));
	}

	/**
	 * Utility function to parse a vector of strings into a two-dimensional
	 * vector class
	 */
	template<class VectorType>
	VectorType parseVector2(std::vector<std::string> strs,
			size_t offset = 1) const {

		assert(strs.size() >= offset + 2);
		return VectorType(
				boost::lexical_cast<typename VectorType::value_type>(
						strs[offset + 0]),
				boost::lexical_cast<typename VectorType::value_type>(
						strs[offset + 1]));
	}
};
