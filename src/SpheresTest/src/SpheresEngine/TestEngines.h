#pragma once

#include <SpheresEngine/Engines.h>

#include <SpheresEngine/AnimationEngine/AnimationEngine.h>
#include <SpheresEngine/InputEngine/InputEngine.h>
#include <SpheresEngine/ResourceEngine/ResourceEngine.h>

/**
 * This class derives from the default render engine
 * and installs a null render backend. This can serves for unit
 * tests where the render engine is not used at all
 */
class TestRenderEngine: public RenderEngine {
public:
	/**
	 * Call the renders engine's ctor and pass an
	 * invalid render backend
	 */
	TestRenderEngine(ResourceEngine & re) :
			RenderEngine(nullptr, re) {

	}
};

/**
 * Class to hold instances of engines for unit testing
 * and which can return the Engines struct often used
 * throughout the code base.
 */
class TestEngines {
public:

	TestEngines() :
			m_render(m_res) {

	}

	Engines getEngines() {
		return Engines(m_entity, m_input, m_render, m_animation);
	}

	ResourceEngine m_res;
	EntityEngine m_entity;
	AnimationEngine m_animation;
	TestRenderEngine m_render;
	InputEngine m_input;
};
