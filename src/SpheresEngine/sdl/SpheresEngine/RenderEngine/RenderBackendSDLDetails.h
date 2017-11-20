#pragma once

#include <SpheresEngine/RenderEngine/RenderBackendBase.h>

#include <SpheresEngine/DataTypes/Resolution.h>

class RenderBackendSDLDetails: public RenderBackendDetails {
public:
	virtual ~RenderBackendSDLDetails() = default;

	explicit RenderBackendSDLDetails(Resolution res) :
			resolution(res) {
	}

	Resolution resolution;
};
