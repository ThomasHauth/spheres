#include "MilkywayBenchmark.h"
#include "CubeBenchmark.h"
#include "ExitBenchmarkInputTransform.h"

#include <SpheresEngine/InputEngine/InputEngine.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/AnimationEngine/AnimationEngine.h>
#include <SpheresEngine/PhysicsEngine/PhysicsEngine.h>
#include <SpheresEngine/ResourceEngine/ResourceEngineAbstract.h>
#include <SpheresEngine/RenderEngine/RenderBackendSDL.h>
#include <SpheresEngine/ThreadedGameLoop.h>
#include <SpheresEngine/Timing.h>
#include <SpheresEngine/Util.h>
#include <SpheresEngine/DataTypes/Resolution.h>

#include <SpheresEngine/InputEngine/SdlSource.h>

#include <SDL2/SDL.h>

#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

/**
 * Instantiate one Benchmark and start the threaded game loop to render it.
 */
int main(int argc, char *argv[]) {

	std::vector < std::string > args;
	for (int i = 0; i < argc; i++) {
		args.emplace_back(std::string(argv[i]));
	}

	bool automated = std::any_of(args.begin(), args.end(),
			[] (std::string & s) {return s == "--automate";});

	std::vector<uniq<BenchmarkBase>> benchmarks;
	benchmarks.push_back(std14::make_unique<MilkywayBenchmark>());
	benchmarks.push_back(std14::make_unique<CubeBenchmark>());

	// todo: select via command line
	auto selectedBenchmark = benchmarks[1].get();

	Resolution resolution(800, 600);

	ResourceEngine resourceE;
	EntityEngine entityEngine;
	AnimationEngine animationEngine;
	PhysicsEngine physicsEngine;
	InputEngine inputEngine;
	RenderEngine re(std14::make_unique<RenderBackendSDL>(resourceE, resolution), resourceE);

	Engines engines(entityEngine, inputEngine, re, animationEngine,
			physicsEngine);

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

	if (automated) {
		gameLoop.setMaxIterations(40);
		gameLoop.setScreenshotFilename("benchmark_out.bmp");
	}

	gameLoop.run();
	re.closeRenderer();
	re.closeDisplay();

	SDL_Quit();
	return 0;
}
