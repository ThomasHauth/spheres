#include "MilkywayBenchmark.h"
#include "CubeBenchmark.h"
#include "ExitBenchmarkInputTransform.h"

#include <SpheresEngine/InputEngine/InputEngine.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/AnimationEngine/AnimationEngine.h>
#include <SpheresEngine/Physics/PhysicsEngine.h>
#include <SpheresEngine/ResourceEngine/ResourceEngineAbstract.h>
#include <SpheresEngine/RenderEngine/RenderBackendSDL.h>
#include <SpheresEngine/ThreadedGameLoop.h>
#include <SpheresEngine/Timing.h>
#include <SpheresEngine/Util.h>

#include <SpheresEngine/InputEngine/SdlSource.h>

#include <SDL2/SDL.h>

#include <random>
#include <vector>

/**
 * Instantiate one Benchmark and start the threaded game loop to render it.
 */
int main() {

	std::vector<uniq<BenchmarkBase>> benchmarks;
	benchmarks.push_back(std14::make_unique<MilkywayBenchmark>());
	benchmarks.push_back(std14::make_unique<CubeBenchmark>());

	// todo: select via command line
	auto selectedBenchmark = benchmarks[1].get();

	ResourceEngine resourceE;
	EntityEngine entityEngine;
	AnimationEngine animationEngine;
	PhysicsEngine physicsEngine;
	InputEngine inputEngine;
	RenderEngine re(std14::make_unique<RenderBackendSDL>(resourceE), resourceE);

	Engines engines(entityEngine, inputEngine, re, animationEngine);

	auto uniqueSdlSource = std14::make_unique<SdlSource>();
	auto ptrSdlSource = uniqueSdlSource.get();
	inputEngine.addSource(std::move(uniqueSdlSource));
	inputEngine.addTransformer(
			std14::make_unique<ExitBenchmarkInputTransform>());

	selectedBenchmark->setupRenderer(re);

	selectedBenchmark->setupScene(engines);

	ThreadedGameLoop gameLoop(re, entityEngine, animationEngine, inputEngine,
			physicsEngine);
	gameLoop.setSdlSource(ptrSdlSource);

	gameLoop.run();
	re.closeRenderer();
	re.closeDisplay();
	return 0;
}
