#pragma once

#include "ParticleSystemVisual.h"

#include <boost/range/irange.hpp>
#include <random>

namespace ParticleSystemModels {

/**
 * Create a fountaint-style particle system
 */
inline void createFountain(ParticleSystemVisual & pv, size_t count,
		std::pair<float, float> velocityRange, Vector3 direction) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(velocityRange.first,
			velocityRange.second);

	std::uniform_real_distribution<> disTransp(50, 255);
	for (auto i[[gnu::unused]] : boost::irange(size_t(0), count)) {
		auto rndNum = dis(gen);

		// asuming the fountain goes in +z direction
		auto rndNumX = dis(gen) * 0.2;
		auto rndNumY = dis(gen) * 0.2;
		auto rndNumZ = dis(gen) * 0.2;

		Vector3 vdirection(direction.x() * rndNum + rndNumX,
		direction.y() * rndNum + rndNumY,
		direction.z() * rndNum + rndNumZ);

		pv.addParticle(ParticleState(Vector3(0, 0, 0), vdirection, 1.0f),
		ParticleColor(0, 0, 255, (GLubyte) disTransp(gen)));
	}

}

/**
 * This is a Particle system model which does no dynamics simulation
 * at all
 */
inline ParticleModelLambda nop() {
	return [](ParticleSystemVisualData & , float ) -> void {
	};
}

/**
 * This is a particle system model which simulates newtonian motion and gravity
 */
inline ParticleModelLambda plainNewtonWithGravity() {
	auto model =
			[](ParticleSystemVisualData & pv, float dt) -> void {

				assert(pv.PositionSizes.size() == pv.SimulationExtra.size());

				for ( auto i[[gnu::unused]] : boost::irange(size_t(0), pv.PositionSizes.size() )) {
					auto & pos = pv.PositionSizes[i];
					auto & sim = pv.SimulationExtra[i];

					float grav = 1.0f;

					sim.Velocity.setY( sim.Velocity.y() -grav * dt);

					auto dx = sim.Velocity.x() * dt;
					auto dy = sim.Velocity.y() * dt;
					auto dz = sim.Velocity.z() * dt;

					pos.setX( pos.x() + dx );
					pos.setY( pos.y() + dy );
					pos.setZ( pos.z() + dz );
				}
			};
	return model;
}

}

