/*
 Copyright (C) 2016 Thomas Hauth. All Rights Reserved.
 * Written by Thomas Hauth (Thomas.Hauth@web.de)

 This file is part o"f Kung Foo Barracuda.

 Kung Foo Barracuda is free software: you can redistribute it and/or modify
 it under the terms o"f the GNU General Public License as published by
 the Free Software Foundation, either version 3 o"f the License, or
 (at your option) any later version.

 Kung Foo Barracuda is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy o"f the GNU General Public License
 along with Kung Foo Barracuda.  I"f not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshRenderer.h>
#include <SpheresEngine/ResourceEngine/MeshLoader.h>

TEST(MeshLoaderTest, parseFaceTuple) {
	MeshLoader ml;

	auto res = ml.parseFaceTuple("3");

	ASSERT_TRUE(std::get < 0 > (res).isValid());
	ASSERT_EQ(3, std::get < 0 > (res).get());
	ASSERT_FALSE(std::get < 1 > (res).isValid());
	ASSERT_FALSE(std::get < 2 > (res).isValid());

	res = ml.parseFaceTuple("3//4");

	ASSERT_TRUE(std::get < 0 > (res).isValid());
	ASSERT_EQ(3, std::get < 0 > (res).get());
	ASSERT_FALSE(std::get < 1 > (res).isValid());
	ASSERT_TRUE(std::get < 2 > (res).isValid());
	ASSERT_EQ(4, std::get < 2 > (res).get());

	res = ml.parseFaceTuple("3/4/5");

	ASSERT_TRUE(std::get < 0 > (res).isValid());
	ASSERT_EQ(3, std::get < 0 > (res).get());
	ASSERT_TRUE(std::get < 1 > (res).isValid());
	ASSERT_EQ(4, std::get < 1 > (res).get());
	ASSERT_TRUE(std::get < 2 > (res).isValid());
	ASSERT_EQ(5, std::get < 2 > (res).get());
}

TEST(MeshLoaderTest, prepare) {

	MeshLoader ld;

	// this mesh data contains two objects, make user we
	// can entangle them
	// Important the vertices defined are shared between all obects
	std::string meshData = "# Blender v2.71 (sub 0) OBJ File: ''\n"
			"# www.blender.org\n"
			"mtllib untitled.mtl\n"
			"o FirstCube\n"
			"v 0.967184 -1.000000 -1.000000\n"
			"v 0.967184 -1.000000 1.000000\n"
			"v -1.032816 -1.000000 1.000000\n"
			"v -1.032816 -1.000000 -1.000000\n"
			"v 0.967184 1.000000 -0.999999\n"
			"v 0.967183 1.000000 1.000001\n"
			"v -1.032816 1.000000 1.000000\n"
			"v -1.032816 1.000000 -1.000000\n"
			"vt 1.0 1.0\n"
			"vn 0.000000 -1.000000 0.000000\n"
			"vn 0.000000 1.000000 0.000000\n"
			"vn 1.000000 -0.000000 0.000000\n"
			"vn -0.000000 -0.000000 1.000000\n"
			"vn -1.000000 -0.000000 -0.000000\n"
			"usemtl Material\n"
			"s off\n"
			"f 2/1/1 3/1/1 4/1/1\n"
			"f 8/1/2 7/1/2 6/1/2\n"
			"f 1/1/3 5/1/3 6/1/3\n"
			"f 2/1/4 6/1/4 7/1/4\n"
			"f 3/1/5 7/1/5 8/1/5\n"
			"f 1/1/6 4/1/6 8/1/6\n"
			"f 1/1/1 2/1/1 4/1/1\n"
			"f 5/1/2 8/1/2 6/1/2\n"
			"f 2/1/3 1/1/3 6/1/3\n"
			"f 3/1/4 2/1/4 7/1/4\n"
			"f 4/1/5 3/1/5 8/1/5\n"
			"f 5/1/6 1/1/6 8/1/6\n"
			"o SecondCube\n"
			"v 0.867184 -1.000000 -3.000000\n"
			"v 0.967184 -1.000000 1.000000\n"
			"v -1.032816 -1.000000 1.000000\n"
			"v -1.032816 -1.000000 -1.000000\n"
			"v 0.967184 1.000000 -0.999999\n"
			"v 0.967183 1.000000 1.000001\n"
			"v -1.032816 1.000000 1.000000\n"
			"v -1.032816 1.000000 -1.000000\n"
			"vt 0.1 1.0\n"
			"vt 0.5 1.0\n"
			"vt 0.25 0.0\n"
			"vt 0.2 0.0\n"
			"vt 0.1 0.0\n"
			"vn 0.000000 -1.000000 0.000000\n"
			"vn 0.000000 1.000000 0.000000\n"
			"vn 1.000000 -0.000000 0.000000\n"
			"vn -0.000000 -0.000000 1.000000\n"
			"vn -1.000000 -0.000000 -0.000000\n"
			"usemtl Material\n"
			"s off\n"
			"f 9/2/1 3/3/1 4/4/1\n"
			"f 8/5/2 7/6/2 6/6/2\n";

	// only one object in the mesh data, take the first
	auto mdMap = ld.loadMesh(meshData);
	ASSERT_EQ(size_t(2), mdMap.size());
	auto firstMesh = mdMap["FirstCube"];
	auto secondMesh = mdMap["SecondCube"];

	const size_t faceStride = 8;
	const size_t faceCount = 12;

	ASSERT_EQ(size_t(faceCount * 3), firstMesh.Position.size());
	ASSERT_EQ(size_t(36), firstMesh.UV.size());

	ASSERT_FLOAT_EQ(0.967184, firstMesh.Position[0].x());
	ASSERT_FLOAT_EQ(1.000000, firstMesh.Position[0].z());

	ASSERT_FLOAT_EQ(-1.000000, firstMesh.Normal[0].y());

	auto flatMesh = firstMesh.asXYZUVNormals();
	ASSERT_FLOAT_EQ(0.967184, flatMesh[0]);
	ASSERT_FLOAT_EQ(1.000000, flatMesh[2]);

	ASSERT_FLOAT_EQ(0.0, flatMesh[5]);
	ASSERT_FLOAT_EQ(-1.000000, flatMesh[6]);

	const size_t faceCountSecond = 2;
	// check the second mesh was also loaded
	ASSERT_EQ(size_t(faceCountSecond * 3), secondMesh.Position.size());
	ASSERT_EQ(size_t(faceCountSecond * 3), secondMesh.UV.size());

	ASSERT_FLOAT_EQ(0.867184, secondMesh.Position[0].x());
	ASSERT_FLOAT_EQ(-3.000000, secondMesh.Position[0].z());
	ASSERT_FLOAT_EQ(-1.000000, secondMesh.Normal[0].y());

	flatMesh = secondMesh.asXYZUVNormals();
	ASSERT_FLOAT_EQ(0.867184, flatMesh[0]);
	ASSERT_FLOAT_EQ(-3.000000, flatMesh[2]);

	// check UV coords
	ASSERT_FLOAT_EQ(0.1, flatMesh[3]);
	// this value is flipped in the blender -> engine conversion
	ASSERT_FLOAT_EQ(0.0, flatMesh[4]);

	ASSERT_FLOAT_EQ(0.5, flatMesh[faceStride + 3]);
	// this value is flipped in the blender -> engine conversion
	ASSERT_FLOAT_EQ(0.0, flatMesh[faceStride + 4]);
}

TEST(MeshLoaderTest, prepareNoUvInfo) {

	MeshLoader ld;

	std::string meshData = "# Blender v2.71 (sub 0) OBJ File: ''\n"
			"# www.blender.org\n"
			"mtllib untitled.mtl\n"
			"o Cube\n"
			"v 0.967184 -1.000000 -1.000000\n"
			"v 0.967184 -1.000000 1.000000\n"
			"v -1.032816 -1.000000 1.000000\n"
			"v -1.032816 -1.000000 -1.000000\n"
			"v 0.967184 1.000000 -0.999999\n"
			"v 0.967183 1.000000 1.000001\n"
			"v -1.032816 1.000000 1.000000\n"
			"v -1.032816 1.000000 -1.000000\n"
			"vn 0.000000 -1.000000 0.000000\n"
			"vn 0.000000 1.000000 0.000000\n"
			"vn 1.000000 -0.000000 0.000000\n"
			"vn -0.000000 -0.000000 1.000000\n"
			"vn -1.000000 -0.000000 -0.000000\n"
			"usemtl Material\n"
			"s off\n"
			"f 2//1 3//1 4//1\n"
			"f 8//2 7//2 6//2\n"
			"f 1//3 5//3 6//3\n"
			"f 2//4 6//4 7//4\n"
			"f 3//5 7//5 8//5\n"
			"f 1//6 4//6 8//6\n"
			"f 1//1 2//1 4//1\n"
			"f 5//2 8//2 6//2\n"
			"f 2//3 1//3 6//3\n"
			"f 3//4 2//4 7//4\n"
			"f 4//5 3//5 8//5\n"
			"f 5//6 1//6 8//6\n";

	auto md = ld.loadMesh(meshData).begin()->second;

	const size_t faceCount = 12;

	ASSERT_EQ(size_t(12 * 3), md.Position.size());
	// no UV coordinates, as they are not in the input file
	ASSERT_EQ(size_t(0), md.UV.size());

	ASSERT_FLOAT_EQ(0.967184, md.Position[0].x());
	ASSERT_FLOAT_EQ(1.000000, md.Position[0].z());

	ASSERT_FLOAT_EQ(-1.000000, md.Normal[0].y());
}

