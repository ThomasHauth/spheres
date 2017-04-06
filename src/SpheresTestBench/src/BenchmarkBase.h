#pragma once

#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/AnimationEngine/AnimationEngine.h>
#include <SpheresEngine/InputEngine/InputEngine.h>

#include "ExitBenchmarkInputTransform.h"

#include <string>

/**
 * Game aspect which takes care of terminating the game
 */
class TerminateBenchmarkAspect: public Aspect<Entity> {
public:
	/**
	 * Create and store the input engine reference
	 */
	TerminateBenchmarkAspect(InputEngine & ie) :
			m_ie(ie) {

	}

	/**
	 * Setup a lambda which checks if there is a new action to terminate the game
	 */
	void init(Entity *) override {
		m_ie.OnNewInputAction.subscribe(
				[]( InputAction * ia )
				{
					logging::Info() << "Received InputAction";
					auto inpAction = dynamic_cast< BenchmarkInputAction * >(ia);
					if (inpAction && inpAction->Action == BenchmarkInputAction::ActionType::Terminate) {
						logging::Info() << "Received ExitBenchmarkInputTransform, terminating";
						exit (0);
					}
				});
	}

private:

	/**
	 * Store the input engine for later usage
	 */
	InputEngine & m_ie;
};

/**
 * Base class for Benchmark implementations. All specific benchmarks
 * need to derive from this class to implement their benchmark scenario.
 */
class BenchmarkBase {
public:

	/**
	 * Provide virtual dtor to support inheritance
	 */
	virtual ~BenchmarkBase() = default;

	/**
	 * The concrete implementation can create the specific render setup
	 * here
	 */
	virtual void setupRenderer(RenderEngine &) = 0;

	/**
	 * The concrete implementation can create all content of the scene in this
	 * method
	 */
	virtual void setupScene(EntityEngine &, RenderEngine &, AnimationEngine &,
			InputEngine &) = 0;

	/**
	 * This method installs the default shaders used for mesh rendering. If a
	 * concrete benchmark needs special shaders, this method nedes to be
	 * overwritten.s
	 */
	virtual void installShaderProgramDefinitions(RenderEngine & re) {
		re.getRenderBackend()->getShaderBackend().addProgramDefinition(
				"default", { { { "vtx", Shader::Type::Vertex }, { "fs",
						Shader::Type::Fragment } } });
		re.getRenderBackend()->getShaderBackend().addProgramDefinition(
				"particles", { { { "particles_vtx", Shader::Type::Vertex }, {
						"particles_fs", Shader::Type::Fragment } } });
	}

	/**
	 * Installs some very common entities, like the one which terminates
	 * the benchmark on keypress.
	 */
	virtual void installCommonEntities(EntityEngine & ee, InputEngine & ie) {

		auto controlEntity = std14::make_unique<Entity>();

		controlEntity->addAspect(
				std14::make_unique<TerminateBenchmarkAspect>(ie));
		ee.addEntity(std::move(controlEntity));
	}

	/**
	 * Returns the name of the benchmark implementation.
	 */
	virtual std::string name() const = 0;
};
