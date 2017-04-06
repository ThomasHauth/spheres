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

#include <boost/math/constants/constants.hpp>

#include "RotateCubeTransform.h"

#include <random>
#include <iostream>

/**
 * Rotates a cube according to user input
 */
class RotateCubeAspect: public Aspect<PositionedEntity> {
public:
	/**
	 * Store the required engine so they can be used later
	 */
	RotateCubeAspect(EntityEngine & ee, InputEngine & ie) :
			m_ee(ee), m_ie(ie) {

	}

	/**
	 * Install lambdas to check for input and continue the cube rotation
	 * on time step
	 */
	void init(PositionedEntity * ent) override {
		m_ie.OnNewInputAction.subscribe(
				[ent, this]( InputAction * ia )
				{
					auto inpAction = dynamic_cast< RotateAction * >(ia);
					if (inpAction) {
						logging::Info() << "Received RotateAction, will adapt rotation speed of cube";
						//ent->resetRotation();
						this->m_lastAxis = inpAction->RotationAxis;
						// todo: something is still weird with the rotation units,
						// therefore this factor of 0.05
						this->m_lastAngle =
						- 0.003f * inpAction->Speed /**
						 *(cur / end)*/
						* 2.0f * boost::math::constants::pi<float>();
					}
				});

		m_ee.OnTimeStep.subscribe([ent, this] (float timeDelta) {

			// rotate by timeslice
				if ( m_lastAxis.length()) {
					ent->rotate( m_lastAxis ,
							m_lastAngle * timeDelta);

					// degrade rotation angle
					m_lastAngle = m_lastAngle - (m_lastAngle * 1.5f *timeDelta);
				}
			});
	}

private:
	/**
	 * Ref to Input Engine
	 */
	InputEngine & m_ie;
	/**
	 * Ref to Entity Engine
	 */
	EntityEngine & m_ee;

	/**
	 * The last axis used for rotation
	 */
	Vector3 m_lastAxis = Vector3::zero();

	/**
	 * The last angle used for rotation
	 */
	float m_lastAngle = 0.0f;
};

/**
 * Benchmark implementation a textured cube which can be rotated by user
 * input.
 */
class CubeBenchmark final: public BenchmarkBase {
public:

	/**
	 * Nothing special needed. Default renderer are setup by the ThreadedGameLoop
	 */
	void setupRenderer(RenderEngine &) override {
	}

	/**
	 * Insert the cube and install the aspects which perform the rotation
	 */
	void setupScene(EntityEngine & ee, RenderEngine & re, AnimationEngine & ae,
			InputEngine & ie) override {
		installShaderProgramDefinitions(re);
		installCommonEntities(ee, ie);

		ie.addTransformer(std14::make_unique<RotateCubeTransform>());

		auto mv1 = new MeshVisual("debug_box", "debug_texture");
		mv1->getData().Center = glm::vec3(0, 0, 0.5);
		auto prepId = re.addToPrepareVisual(mv1);

		auto boxEntity = std14::make_unique<PositionedEntity>();
		auto boxEntityPtr = boxEntity.get();
		boxEntity->addVisualPlaceholder(prepId);
		boxEntity->addAspect(std14::make_unique<RotateCubeAspect>(ee, ie));
		ee.addEntity(std::move(boxEntity));

		auto cameraEntity = std14::make_unique<CameraEntity>();
		cameraEntity->lookAt(Vector3::zero());
		cameraEntity->setPosition(Vector3(5, 3, 0));
		ee.addEntity(std::move(cameraEntity));

		// add animation to rotate the galaxy
		auto lmbRotateCube = [boxEntityPtr](float , float )
		{
			/*	boxEntityPtr->resetRotation();
			 boxEntityPtr->rotate( Vector3(1.0, 0.8,0.6),
			 // todo: something is still weird with the rotation units,
			 // therefore this factor of 0.05
			 0.05f *
			 (cur / end)
			 * 2.0f * boost::math::constants::pi<float>());*/
		};

		Animation an( { { 0.0f, Sequence(100.0f, lmbRotateCube) } }, true);
		ae.addAnimation(an, 0.5f);

	}

	/**
	 * return name of this benchmark
	 */
	std::string name() const override {
		return "cube";
	}

};
