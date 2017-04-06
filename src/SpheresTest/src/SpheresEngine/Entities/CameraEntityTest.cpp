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

#include <SpheresEngine/EntityEngine/CommonEntities/CameraEntity.h>

TEST(CameraEntityTest, positionOnVisual) {
	CameraEntity ce;
	VisualDataExtractContainer exCon;

	ce.lookAt(Vector3(10, 10, 10));
	ce.setPosition(Vector3(50, 50, 50));

	ce.extractData(exCon);

	ASSERT_FLOAT_EQ(10, exCon.CameraData.LookAt.x());
	ASSERT_FLOAT_EQ(50, exCon.CameraData.Position.x());
}

