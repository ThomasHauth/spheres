#pragma once

#include <boost/noncopyable.hpp>
#include <SpheresEngine/Threading/AtomicDoubleBuffer.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/Visuals/VisualDataExtract.h>

#include <functional>
#include <memory>
#include <thread>
#include <mutex>

class StateTransitions;
class SdlSource;
class InputEngine;
class PhysicsEngine;
class EntityEngine;
class AnimationEngine;

/**
 * Modular game loop which support multi-threading.
 */
class ThreadedGameLoop: boost::noncopyable {
public:
	/**
	 * Constructor which takes all the various engines as reference
	 * and stores them for later use.
	 */
	ThreadedGameLoop(RenderEngine & re,/* StateTransitions & trans,*/
	EntityEngine& ee, AnimationEngine & an, InputEngine & input,
			PhysicsEngine & phys, bool profile = false,
			std::string profileFileName = "") :
			m_renderEngine(re), //
			m_entityEngine(ee),	//
			m_animationEngine(an),	//
			m_inputEngine(input), //
			m_physics(phys),	//
			m_profile(profile),	//
			m_profileFileName(profileFileName) {
	}

	/**
	 * virtual dtor to support inheritance
	 */
	virtual ~ThreadedGameLoop() = default;

	/** can be used in an application which does not want to return control
	 * this will be used for the linux compile
	 * the android version will request the lambdas below and call the specific
	 * code directly, because there, we don't always own the game loop
	 */
	virtual void run();

	/**
	 * Generates and return the lambda function containing the game logic
	 */
	std::function<void(float)> getLogicLambda();

	/**
	 * Generates and returns the lambda function to initialize the render engine
	 */
	std::function<void()> getRenderInitLambda();

	/**
	 * Generates and returns the lambda function setting up the renderers
	 */
	std::function<void()> getAddRendererLambda();

	/**
	 * Generates and returns the lambda function to render the scene
	 */
	std::function<void(float)> getRenderLambda();

#ifdef USE_SDL
	/**
	 * If SDL is used, this method needs to be used to set the input source
	 */
	void setSdlSource(SdlSource * sdlInputSource) {
		m_sdlInputSource = sdlInputSource;
	}
#endif

private:

	/**
	 * Reference to the render engine
	 */
	RenderEngine & m_renderEngine;

	/**
	 * Reference to the entity engine
	 */
	EntityEngine & m_entityEngine;

	/**
	 * Reference to the animation engine
	 */
	AnimationEngine & m_animationEngine;

	/**
	 * Reference to the input engine
	 */
	InputEngine & m_inputEngine;

	/**
	 * Reference to the physics engine
	 */
	PhysicsEngine &m_physics;

	/**
	 * This mutex needs to be locked when accessing the
	 * m_extractContainer member
	 */
	std::mutex m_extractContainerAccess;

	/**
	 * Contains the visual data which has been extracted for rendering
	 */
	VisualDataExtractContainer m_extractContainer;

	/**
	 * This mutex needs to be locked when accessing the
	 * m_preparedVisualsAccess member
	 */
	std::mutex m_preparedVisualsAccess;

	/**
	 * Contains a list of all all visuals which have been preperad
	 * in the last iteration of the render loop
	 */
	RenderEngine::PrepareVisualList m_preparedVisuals;

	/**
	 * This mutex needs to be locked when accessing the
	 * m_rendererVisualChanges member
	 */
	std::mutex m_rendererVisualChangesAccess;

	/**
	 * Contains a the visual changes done during the render thread
	 * which need to be stored back into the visuals attached to
	 * entities
	 */
	std::vector<RendererVisualChange> m_rendererVisualChanges;

	/**
	 * Maximum frame rate in frames / second
	 */
	const float m_maxFramerate = 100.0f;

	/**
	 * Maximum rate (calls / second) of the game loop
	 */
	const float m_maxGameloopRate = 60.0f;

	/**
	 * Minimum time spend in the render loop
	 */
	const float m_minTime = 1.0f / m_maxFramerate;

	/**
	 * Profile information will be logged if true
	 */
	const bool m_profile = false;

	/**
	 * The file name the profile output will be written to
	 */
	std::string m_profileFileName;

	/**
	 * The rate with which profile information is dumped, meaning
	 * every ith frame where i = m_profileDumpRate
	 */
	const size_t m_profileDumpRate = 100;

	/**
	 * Set to true to signal render and logic thread to terminate
	 */
	std::atomic<bool> m_terminate;

#ifdef USE_SDL
	/**
	 * Stores a pointer to the SDL Source object for device input
	 */
	SdlSource * m_sdlInputSource = nullptr;
#endif
};
