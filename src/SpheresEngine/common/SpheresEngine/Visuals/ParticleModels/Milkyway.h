#pragma once

#include <SpheresEngine/VectorTypes.h>
#include <SpheresEngine/Visuals/ParticleSystemVisual.h>

#include <utility>
#include <cstddef>

namespace milkyway {

/**
 * Typedef to store one color point on a linear scale for mixing
 */
typedef std::pair<float, ParticleColor> ColorPoint;

/**
 * List of colors which can be used to interpolate colors on a linear scale
 */
typedef std::vector<ColorPoint> ColorInterpolation;

/**
 * Interpolate a color using the key points and the radius of the actual
 * particle being generated
 */
ParticleColor interpolateColor(ColorInterpolation & points, float radius);

/**
 * Create a full particle system looking like the milky way
 * @param count the number of particle in the milkyway
 */
void createMilkyway(ParticleSystemVisual & pv, const size_t count);

}
