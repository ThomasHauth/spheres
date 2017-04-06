#include "Milkyway.h"

#include <SpheresEngine/Util.h>

#include <random>
#include <algorithm>

#include <boost/range/irange.hpp>
#include <boost/math/constants/constants.hpp>

namespace milkyway {

ParticleColor interpolateColor(ColorInterpolation & points, float radius) {

	// find the two closets points
	util::ValidValue<ParticleColor> lower;
	util::ValidValue<ParticleColor> upper;

	float lower_r;
	float upper_r;
	for (auto pp : points) {
		if (pp.first < radius) {
			lower.setValue(pp.second);
			lower_r = pp.first;
		}
		if ((pp.first > radius) && (!upper.isValid())) {
			upper.setValue(pp.second);
			upper_r = pp.first;
		}
	}

	if (!lower.isValid())
		return upper.get();
	if (!upper.isValid())
		return lower.get();

	float rel_distance_to_lower = (radius - lower_r) / (upper_r - lower_r);
	float rel_distance_to_upper = 1.0f - rel_distance_to_lower;

	return ParticleColor(
			(lower.get().R * rel_distance_to_upper
					+ upper.get().R * rel_distance_to_lower),
			(lower.get().G * rel_distance_to_upper
					+ upper.get().G * rel_distance_to_lower),
			(lower.get().B * rel_distance_to_upper
					+ upper.get().B * rel_distance_to_lower),
			(lower.get().V * rel_distance_to_upper
					+ upper.get().V * rel_distance_to_lower));
}

void createMilkyway(ParticleSystemVisual & pv, const size_t count/*,
 std::pair<float, float> velocityRange, Vector3 direction*/) {

	const float outgoingArmAdditionTurn = 5.0f;
	const float outgoingArmOutgoingFactor = 4.0f;

	auto lmdArmsRPhiRelation =
			[&] ( float barWidth, float phi /* range 0 to inf*/) -> float {

				const auto armTurnRatio = phi / ( boost::math::constants::pi<float>() *2.0f );

				return (barWidth*0.5f) +
				(armTurnRatio) * outgoingArmAdditionTurn + std::pow(armTurnRatio,outgoingArmOutgoingFactor);
			};

	const auto colorYellowish = ParticleColor(240, 255, 155, 255);
	const auto colorWhiteish = ParticleColor(206, 206, 206, 255);
	const auto colorRedish = ParticleColor(240, 20, 20, 255);
	const auto colorPinkish = ParticleColor(219, 76, 138, 255);

	const float maxRadius = 9.0f;
	const float armLocationPhiMax = 2.3f;

	// modeled as a cylinder
	const float centralBarWidth = 2.0f;
	const float centralBarDiameter = 0.3f;

	// fuzyness in xy-plane in units of maxRadius
	float fuzzyness_xy_plane = 0.01f;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> circleDist(0,
			boost::math::constants::pi<float>() * 2.0f);

	std::uniform_real_distribution<float> armLocationPhiDist(0,
			boost::math::constants::pi<float>() * armLocationPhiMax);

	// increase the probability from 0 to 5
	// remain flat from 5 to 10
	// decrease from 10 to 15 at the same rate
	const float centralBarHalfWidth = centralBarWidth * 0.5;
	std::vector<float> centralBarDistI { -centralBarHalfWidth * 1.2f,
			-centralBarHalfWidth, centralBarHalfWidth, centralBarHalfWidth
					* 1.2f };
	std::vector<float> centralBarDistW { 0, 1, 1, 0 };
	std::piecewise_linear_distribution<> centralBarXDist(
			centralBarDistI.begin(), centralBarDistI.end(),
			centralBarDistW.begin());

	const float centralBarRadius = centralBarDiameter * 0.5f;
	std::vector<float> centralBarRadiusI { 0.0f, centralBarRadius,
			centralBarRadius * 2.0f };
	std::vector<float> centralBarRadiusW { 1, 1, 0 };
	std::piecewise_linear_distribution<> centralBarRadiusDist(
			centralBarRadiusI.begin(), centralBarRadiusI.end(),
			centralBarRadiusW.begin());

	std::uniform_real_distribution<float> polarPhiDist(0,
			boost::math::constants::pi<float>() * 2.0f);

	// fuzzyness in xy-plane
	const float xyFuzzynessSigma = 0.1f;
	std::normal_distribution<float> xyPlaneFuzzynessDist(1.0f,
			xyFuzzynessSigma);
	std::normal_distribution<float> xyPlaneFuzzynessRedDist(1.0f,
			xyFuzzynessSigma * 0.2f);

	std::normal_distribution<float> normDist(0.0, 4.5 /*sigma*/);
	std::normal_distribution<float> normDistHeight(0.0, 0.2 /*sigma*/);

	ColorInterpolation colorTable;
// center is yellowish
	colorTable.push_back(
			std::make_pair(0.0, ParticleColor(240, 255, 155, 255)));
// outer parts are mor blueish / grayish
	colorTable.push_back(
			std::make_pair(1.0, ParticleColor(206, 206, 206, 255)));
	colorTable.push_back(
			std::make_pair(4.0, ParticleColor(138, 145, 166, 255)));
	colorTable.push_back(
			std::make_pair(8.0, ParticleColor(120, 100, 110, 255)));

	// central bar in the center
	for (size_t addedCount = 0; addedCount < (count / 10);) {

		float x = centralBarXDist(gen);
		float radius = centralBarRadiusDist(gen);
		float phi = polarPhiDist(gen);

		float y = std::sin(phi) * radius;
		float z = std::cos(phi) * radius;

		pv.addParticle(ParticleState(Vector3(x, y, z), Vector3::zero(), 1.0f),
				colorYellowish);

		addedCount++;
	}

	for (size_t addedCount = 0; addedCount < count;) {

		auto phi = armLocationPhiDist(gen);
		auto radius = lmdArmsRPhiRelation(centralBarWidth, phi);

		auto fuzyness = xyPlaneFuzzynessDist(gen);
		auto relFuzynessPull = (1.0f - fuzyness) / xyFuzzynessSigma;

		radius *= fuzyness;	// * (0.7 + (relativeDistance -0.3));

		float y = normDistHeight(gen);
		float z, x;
		if ((addedCount % 2) == 0) {
			z = std::sin(phi) * radius;
			x = std::cos(phi) * radius + centralBarWidth * 0.3f;
		} else {
			z = std::sin(-phi) * radius;
			x = -std::cos(-phi) * radius - centralBarWidth * 0.3f;
		}

		ColorInterpolation colorTableArms;
		// center is yellowish
		colorTable.push_back(std::make_pair(0.0, colorYellowish));
		// outer parts are mor blueish / grayish
		colorTable.push_back(std::make_pair(2.0, colorWhiteish));

		pv.addParticle(ParticleState(Vector3(x, y, z), Vector3::zero(), 1.0f),
				interpolateColor(colorTable, std::abs(relFuzynessPull)));

		addedCount++;
	}

	// red specacles, pink spectacles
	for (size_t addedCount = 0; addedCount < (count / 10);) {

		auto phi = armLocationPhiDist(gen);
		auto radius = lmdArmsRPhiRelation(centralBarWidth, phi);

		auto fuzyness = xyPlaneFuzzynessRedDist(gen);

		radius *= (fuzyness);	// * (0.7 + (relativeDistance -0.3));

		float y = normDistHeight(gen);
		float z, x;
		if ((addedCount % 2) == 0) {
			z = std::sin(phi) * radius;
			x = std::cos(phi) * radius + centralBarWidth * 0.3f;
		} else {
			z = std::sin(-phi) * radius;
			x = -std::cos(-phi) * radius - centralBarWidth * 0.3f;
		}

		ColorInterpolation colorTableArms;
		// center is yellowish
		colorTable.push_back(std::make_pair(0.0, colorYellowish));
		// outer parts are mor blueish / grayish
		colorTable.push_back(std::make_pair(2.0, colorWhiteish));

		ParticleColor specColor;

		if ((addedCount % 2) == 0)
			specColor = colorRedish;
		else
			specColor = colorPinkish;

		pv.addParticle(ParticleState(Vector3(x, y, z), Vector3::zero(), 1.0f),
				specColor);

		addedCount++;
	}

	for (size_t addedCount = 0; addedCount < count;) {
		float particleRadius = std::abs(normDist(gen));

		if (particleRadius > maxRadius)
			continue;

		float onCircle = circleDist(gen);

		Vector3 vdirection;
		auto pcolor = interpolateColor(colorTable, particleRadius);

		float px = std::sin(onCircle) * particleRadius
				+ xyPlaneFuzzynessDist(gen);
		float pz = std::cos(onCircle) * particleRadius
				+ xyPlaneFuzzynessDist(gen);
		float py = normDistHeight(gen);

		pv.addParticle(ParticleState(Vector3(px, py, pz), vdirection, 1.0f),
				pcolor);
		addedCount++;
	}
}
}
