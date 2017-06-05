#pragma once

#include "BenchmarkBase.h"

#include <SpheresEngine/Util.h>
#include <SpheresEngine/Visuals/ParticleSystemVisual.h>
#include <SpheresEngine/Visuals/ParticleSystemModels.h>
#include <SpheresEngine/Visuals/ParticleModels/Milkyway.h>
#include <SpheresEngine/RenderEngine/Targets/CameraTarget.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshRenderer.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ParticlesRenderer.h>
#include <SpheresEngine/EntityEngine/CommonEntities/PositionedEntity.h>
#include <SpheresEngine/EntityEngine/CommonEntities/CameraEntity.h>

#include <random>
#include <iostream>

/**
 * Benchmark class which generates a milkyway with many stars using a
 * particle system
 */
class MilkywayBenchmark final: public BenchmarkBase {
public:

	/**
	 * Nothing special needed. Default renderer are setup by the ThreadedGameLoop
	 */
	void setupRenderer(RenderEngine &) override {
	}

	/**
	 * Generate a particle system for the milkyway and generate and animation
	 * to rotate the galaxy.
	 */
	void setupScene(Engines & engines) override {

		installShaderProgramDefinitions(engines.render);
		installCommonEntities(engines);
		/*
		 MeshVisual mv1("debug_box", "debug_texture");
		 mv1.getData().Center = glm::vec3(0, 0, 0.5);
		 re.prepareVisual(mv1);
		 */
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-1.0f, 1.0f);

		//ParticleSystemVisual pv1(ParticleSystemModels::nop());

		// do not simulate particle movement inside the galaxy right now
		// so the particle positions do not need to be downloaded to GPU buffers
		m_particleVisual = std14::make_unique<ParticleSystemVisual>(
				ParticleSystemModels::nop());
		milkyway::createMilkyway(*m_particleVisual.get(), 5000);

		// some one needs to own this visual objects here !
		//re.prepareVisual(*m_particleVisual.get());

		// the visual will not become available right now, but only once it was been
		// prepared ...
		auto prepId = engines.render.addToPrepareVisual(m_particleVisual.get());

		auto galaxyEntity = std14::make_unique<PositionedEntity>();
		galaxyEntity->addVisualPlaceholder(prepId); // (m_particleVisual.get());
		auto galaxyEntityPtr = galaxyEntity.get();
		engines.entity.addEntity(std::move(galaxyEntity));

		auto cameraEntity = std14::make_unique<CameraEntity>();
		cameraEntity->lookAt(Vector3::zero());
		cameraEntity->setPosition(Vector3(8, -8, 8));
		//cameraEntity->setPosition(Vector3(1, 15, 1));
		//cameraEntity->setPosition(Vector3(8, 0, 8));
		engines.entity.addEntity(std::move(cameraEntity));

		// add animation to rotate the galaxy
		auto lmbRotateGalaxy = [galaxyEntityPtr](float cur, float end)
		{
			if (galaxyEntityPtr->getVisuals().size() == 0)
			return;
			auto ps = galaxyEntityPtr->getFirstVisual<ParticleSystemVisual>();
			// write rotational information into the user data of the shader
				ps->getData().UserData.Floats[0] = cur / end;
			};

		Animation an( { { 0.0f, Sequence(10.0f, lmbRotateGalaxy) } }, true);
		engines.animation.addAnimation(an, 0.1f);
	}

	/**
	 * return name of this benchmark
	 */
	std::string name() const override {
		return "milkyway";
	}

	/**
	 * Stores the particle system of the galaxy
	 */
	std::unique_ptr<ParticleSystemVisual> m_particleVisual;

};
