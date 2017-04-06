#pragma once

#include "VisualBase.h"
#include "VisualDataBase.h"
#include <SpheresEngine/VectorTypes.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>

#include <string>
#include <vector>

struct VisualDataExtractContainer;

/**
 * Extra information stored with each particle to perform the proper
 * dynamics simulation
 */
struct ParticleSimulationExtra {

	/**
	 * Create a new extra object
	 */
	ParticleSimulationExtra(Vector3 velocity, float mass) :
			Velocity(velocity), Mass(mass) {

	}

	/**
	 * 3d velocity of the particle
	 */
	Vector3 Velocity;

	/**
	 * Mass of this particle
	 */
	float Mass;

	/**
	 * Lifetime is seconds to simulate particle decay
	 * can be negative, if a particle does not exist atm.
	 */
	float Lifetime = 1.0f;
};

/**
 * structure to store the particle state
 */
struct ParticleState: ParticleSimulationExtra {

	/**
	 * Create the particle state object
	 */
	ParticleState(Vector3 position, Vector3 velocity, float mass) :
			ParticleSimulationExtra(velocity, mass), Position(position) {
	}

	/**
	 * position of this particle in the wordl system
	 */
	Vector3 Position;
};

/**
 * This struct stores the color of one particle
 */
struct ParticleColor {

	/**
	 * Generate a new particle color with RGB color and alpha channel
	 */
	ParticleColor(GLubyte r, GLubyte g, GLubyte b, GLubyte v) :
			R(r), G(g), B(b), V(v) {
	}

	/**
	 * default ctor which generates a while particle
	 */
	ParticleColor() = default;

	/**
	 * Red value: from 0 to 255
	 */
	GLubyte R = 255;

	/**
	 * Green value: from 0 to 255
	 */
	GLubyte G = 255;

	/**
	 * Blue value: from 0 to 255
	 */
	GLubyte B = 255;

	/**
	 * Transparent value: from 0 to 255
	 */
	GLubyte V = 255;
};

/**
 * Visual data extracted for particle system rendering
 */
struct ParticleSystemVisualData: LocatedVisualDataBase, ShadedVisualDataBase {

	/**
	 * Typedef for a 3d-vector which also stores the payload particle size,
	 * which is of type float
	 */
	typedef Vector3PayloadBase<float> Vector3WithSize;

	/** if this is true, the three std::vectors below
	 * contain entries and the Renderer must download the
	 * new position to the GPU. This happens for example
	 * when a CPU-based animation of the particle system is computed
	 * otherwise no download will be performed (much faster)
	 */
	bool Updated = false;

	/**
	 * Stores the 3d position and the size of all Particles
	 */
	std::vector<Vector3WithSize> PositionSizes;

	/**
	 * Stores the color all Particles
	 */
	std::vector<ParticleColor> Color;

	/**
	 * Stores additional simulation data for each particle
	 */
	std::vector<ParticleSimulationExtra> SimulationExtra;

	/**
	 * Return a pointer to the buffer which holds the position and size
	 * of the particles
	 */
	GLfloat * getPosSizeBuffer() const {
		return (GLfloat *) PositionSizes.data();
	}

	/**
	 * Return a pointer to the buffer which holds the color code
	 * of each particle
	 */
	GLubyte* getColorBuffer() const {
		return (GLubyte *) Color.data();
	}

	/**
	 * Return the number of particles in the this particle system
	 */
	size_t getParticleCount() const {
		return PositionSizes.size();
	}

	/**
	 * Id of the buffer holding the particle position
	 */
	GLuint vertexPositionBuffer;

	/**
	 * Id of the buffer holding the color for all particles
	 */
	GLuint vertexColorBuffer;
};

/**
 * typedef for concrete particle system implementations
 */
typedef std::function<void(ParticleSystemVisualData & pv, float dt)> ParticleModelLambda;

/**
 * Visual which holds all content (including the simulation model) for a particle system
 */
class ParticleSystemVisual: public VisualBase<ParticleSystemVisualData> {
public:

	/**
	 * default constructor, wilth no particles and simulation model
	 */
	ParticleSystemVisual() = default;

	/**
	 * Create a particle system with no initial particles, but
	 * a simulation model.
	 */
	ParticleSystemVisual(ParticleModelLambda model) :
			m_model(model) {
	}

	/**
	 * Return the name of this visual
	 */
	std::string getType() const override {
		return "ParticleSystemVisual";
	}

	/**
	 * Checks if the shader has been reloaded and the shader id needs to be updated
	 */
	void update(RendererVisualChange const& vc) override;

	/**
	 * Add a new particle to this particle system
	 */
	void addParticle(ParticleState ps, ParticleColor pc = ParticleColor()) {
		auto p = ParticleSystemVisualData::Vector3WithSize(ps.Position.x(),
				ps.Position.y(), ps.Position.z(), 0.1f);

		getData().PositionSizes.emplace_back(p);

		auto sim = ParticleSimulationExtra(ps.Velocity, ps.Mass);

		getData().SimulationExtra.emplace_back(sim);

		getData().Color.push_back(pc);
		getData().Updated = true;
	}

	/**
	 * Returns the number of particles in this particle system
	 */
	size_t getParticleCount() const {
		return getData().getParticleCount();
	}

	/**
	 * Executes one simulation step with the registered particle system model
	 */
	void step(float deltaT);

	/**
	 * Extract the data used for rendering all particles
	 */
	void extractData(VisualDataExtractContainer & cont, ExtractOffset const& eo)
			override;

private:
	/**
	 * Holds the lambda function which can execute the particle system's model
	 */
	ParticleModelLambda m_model;
};
