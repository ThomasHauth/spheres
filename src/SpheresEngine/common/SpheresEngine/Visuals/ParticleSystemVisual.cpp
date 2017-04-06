#include "ParticleSystemVisual.h"

#include <SpheresEngine/Visuals/VisualDataExtract.h>

//#include "../../../SpheresEngine/src/Visuals/VisualDataExtract.h"

void ParticleSystemVisual::extractData(VisualDataExtractContainer & cont,
		ExtractOffset const& eo) {

	// todo: use ExtractOffset

	auto data = getData();
	// recenter on entity Position
	data.Center = eo.Position.toGlm();
	data.Rotation = eo.Rotation;

	cont.ParticleSystems.emplace_back(data);

	// mark that there is no pending update on the particles
	// data (yet), because no modifications form the CPU side have
	// been performed on the particle position/color
	getData().Updated = false;
}

void ParticleSystemVisual::step(float deltaT) {
	// recompute positions and stuff

	/*for (auto & p : getData().Particles) {
	 // compute equation of motion etc.
	 }*/
	m_model(getData(), deltaT);
}

void ParticleSystemVisual::update(RendererVisualChange const& vc) {
	if (vc.EventType
			== RendererVisualChange::EventTypeEnum::ShaderProgramReload) {
		// is this our shader ?
		if (vc.ShaderProgramName == this->getData().Shader.getName()) {
			// update id of shader program
			this->getData().Shader.setId(vc.ShaderProgramId);
			logging::Info() << "Update shader program " << vc.ShaderProgramName
					<< " in visual data";
		}
	}
}
