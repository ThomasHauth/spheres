#pragma once

#include <SpheresEngine/Engines.h>
//#include <SpheresEngine/EntityEngine/CommonEntities/PositionedEntit

#include <SpheresEngine/Visuals/MeshVisual.h>

class EntityFactory {
public:
	EntityFactory(Engines & eg) :
			m_engines(eg) {

	}

	struct MeshVisualConfig {
		MeshVisualConfig(std::string meshFileName, std::string textureName,
				util::ValidValue<std::string> optionalObjectName =
						util::ValidValue<std::string>::invalid()) :
				MeshFileName(meshFileName), TextureName(textureName), ObjectName(
						optionalObjectName) {
		}

		std::string MeshFileName;
		util::ValidValue<std::string> ObjectName;
		std::string TextureName;
	};

	template<class TEntity>
	std::unique_ptr<TEntity> createEntity(
			std::vector<MeshVisualConfig> meshConfigs) {

		auto entity = std14::make_unique<TEntity>();

		for (auto meshConf : meshConfigs) {
			auto mesh_visual = new MeshVisual(meshConf.MeshFileName,
					meshConf.TextureName);
			mesh_visual->getData().Visible = true;
			auto prepId_mesh = m_engines.render.addToPrepareVisual(mesh_visual);

			entity->addVisualPlaceholder(prepId_mesh);

		}
		//wandEntity->setPosition(Vector3(3.5, 3.5, -4.5));
		//auto wandEntityPtr = wandEntity.get();
		//wandEntity->addVisualPlaceholder(prepId_wand);

		return entity;
	}

private:

	Engines & m_engines;

}
;
