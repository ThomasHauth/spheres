#pragma once

#include <SpheresEngine/VectorTypes.h>

/**
 * Target data used by the CameraTarget
 */
struct CameraTargetData {
	/**
	 * The position of the camera
	 */
	Vector3 Position;

	/**
	 * The location the camera looks at
	 */
	Vector3 LookAt;
};
