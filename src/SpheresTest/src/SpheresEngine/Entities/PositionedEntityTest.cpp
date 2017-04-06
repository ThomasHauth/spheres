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

#include <SpheresEngine/EntityEngine/CommonEntities/PositionedEntity.h>
#include <SpheresEngine/Visuals/MeshVisual.h>
#include <SpheresEngine/Util.h>

#include <boost/math/constants/constants.hpp>

#include <memory>

TEST(PositionedEntityTest, positionOnVisual) {
	PositionedEntity pe;
	auto ms = std14::make_unique<MeshVisual>("mesh_name", "text_name");
	VisualDataExtractContainer exCon;

	pe.setPosition(Vector3(50, 50, 50));
	pe.addVisual(ms.get());

	pe.extractData(exCon);

	ASSERT_FLOAT_EQ(50, exCon.MeshVisuals[0].Center.x);

}

TEST(PositionedEntityTest, rotationOnVisual) {
	PositionedEntity pe;
	auto ms = std14::make_unique<MeshVisual>("mesh_name", "text_name");
	VisualDataExtractContainer exCon;

	// no rotation at all in the first test
	pe.setRotation(Quaternion(glm::vec3(0.0f, 0.0f, 0.0f)));
	pe.addVisual(ms.get());

	pe.extractData(exCon);

	ASSERT_FLOAT_EQ(0, glm::yaw(exCon.MeshVisuals[0].Rotation));

	// rotate around the roll axis by 90 degrees
	pe.rotate(Vector3(0.0, 0.0, 20.0),
			boost::math::constants::pi<float>() * 0.5f);
	exCon.clear();
	pe.extractData(exCon);

	ASSERT_FLOAT_EQ(0, glm::pitch(exCon.MeshVisuals[0].Rotation));
	ASSERT_GE(glm::roll(exCon.MeshVisuals[0].Rotation), 0.1f);
	ASSERT_FLOAT_EQ(0, glm::yaw(exCon.MeshVisuals[0].Rotation));
}
