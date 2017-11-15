#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <SpheresEngine/ResourceEngine/MeshLoader.h>

std::map<std::string, MeshData> MeshLoader::loadMesh(std::string meshData) {
	MeshData md;
	std::map<std::string, MeshData> mdMap;

	struct MdRawStruct {
		std::vector<Vector3> vtxPos;
		std::vector<Vector3> vtxNormal;
		std::vector<Vector2> uv;
	};

	// Important: This contains the vertex, normal and uv data loaded from one obj
	// The indexing for different objects in one file is on the total list of vertex data contained
	// in the file !
	MdRawStruct mdRaw;

	std::vector < std::string > lines;
	boost::split(lines, meshData, boost::is_any_of("\n"));

	util::ValidValue<std::string> thisObjectName;

	for (std::string sLine : lines) {
		std::vector < std::string > strs;
		boost::split(strs, sLine, boost::is_any_of(" "));

		if (strs.size() > 1) {
			if (strs[0] == "o") {
				if (thisObjectName.isValid()) {
					mdMap[thisObjectName.get()] = md;
					/// crate new MeshData instance for this object, but keep the
					// content of MdRawStruct, as the vertex position is shared
					// between objects in one file.
					md = MeshData();
				}
				// store the object name of the mesh data to follow
				thisObjectName = strs[1];
			}
		}

		if (strs.size() > 3) {
			if (strs[0] == "v") {
				auto p = parseVector<Vector3>(strs);
				mdRaw.vtxPos.push_back(parseVector<Vector3>(strs));
			} else if (strs[0] == "vn") {
				mdRaw.vtxNormal.push_back(parseVector<Vector3>(strs));
			} else if (strs[0] == "f") {
				std::vector< PosTexNormalTuple > face_info_list = { {parseFaceTuple(strs[1]), parseFaceTuple(
								strs[2]), parseFaceTuple(strs[3])}};

				// compile the vertex positions, uv values and normals
				// for all three coordinates forming the trinangle's face
				for ( auto face_info: face_info_list ) {
					auto facePos = mdRaw.vtxPos[std::get < 0 > (face_info).get() - 1];
					util::ValidValue<Vector2> faceUv;

					if (std::get < 1 > (face_info).isValid() ) {
						faceUv.setValue( mdRaw.uv[std::get < 1 > (face_info).get() - 1] );
					}
					auto normalPos = mdRaw.vtxNormal[std::get < 2> (face_info).get() - 1];

					md.Position.push_back ( facePos);
					if ( faceUv.isValid() ) {
						md.UV.push_back ( faceUv.get());
					}
					md.Normal.push_back(normalPos);
				}
			}
		} else if ( strs.size() == 3 ) {
			if (strs[0] == "vt") {
				mdRaw.uv.push_back(parseVector2<Vector2>(strs));
			}
		}
	}

	// store the last loaded object
	if (thisObjectName.isValid()) {
		mdMap[thisObjectName.get()] = md;
	}

	for (auto & mdItem: mdMap){
		logging::Info() << "Mesh object with name " << mdItem.first << " loaded";
	}
	logging::Info() << "Mesh Data of " << mdMap.size() << " meshes loaded";

	return mdMap;
}

MeshLoader::PosTexNormalTuple MeshLoader::parseFaceTuple(std::string ft) const {

	std::vector < std::string > strs;
	boost::split(strs, ft, boost::is_any_of("/"));

// only one entry ?
// then this is only the index of the position
	if (strs.size() == 1) {
		return std::make_tuple(boost::lexical_cast<int>(strs[0]),
				util::ValidValue<int>(), util::ValidValue<int>());
	}
// two entries? this is the index of the position and the texture pos
	else if (strs.size() == 2) {
		return std::make_tuple(boost::lexical_cast<int>(strs[0]),
				boost::lexical_cast<int>(strs[1]), util::ValidValue<int>());
	}
// three entries? this is index of the position, texture pos and normal pos
	else if (strs.size() == 3) {

// texture pos can be "" if no texture has been assigned
		auto texturePos =
				strs[1] == "" ?
						util::ValidValue<int>::invalid() :
						util::ValidValue<int>() :
						boost::lexical_cast<int>(strs[1]);

		return std::make_tuple(boost::lexical_cast<int>(strs[0]), texturePos,
				boost::lexical_cast<int>(strs[2]));
	}

// all invalid ...
	return std::make_tuple(util::ValidValue<int>(), util::ValidValue<int>(),
			util::ValidValue<int>());
}

