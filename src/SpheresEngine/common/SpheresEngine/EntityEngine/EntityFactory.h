#pragma once

#include <SpheresEngine/Engines.h>
#include <SpheresEngine/Visuals/MeshVisual.h>

#include <type_traits>

class CollisionHandler {
public:

	template<typename T, typename std::enable_if<
			std::is_base_of<CollisionMixin, T>::value>::type* = nullptr>
	static void setup(Engines & engines, T& ent,
			CollisionMixin::ShapeSource shapesource) {
		ent.setShapeSource(shapesource);
		ent.setBoxSize(Vector3(1.0f, 1.0f, 1.0f));

		engines.physics.addEntity(&ent);

		logging::Info() << "Registering Entity for physics collisions";
	}

	static void setup(Engines &, Entity&, CollisionMixin::ShapeSource) {
		logging::Info()
				<< "Skipping physics collision setup as respective mixin is not available";
	}

};

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
			std::vector<MeshVisualConfig> meshConfigs,
			util::ValidValue<CollisionMixin::ShapeSource> collisionShape =
					util::ValidValue<CollisionMixin::ShapeSource>()) {

		auto entity = std14::make_unique<TEntity>();

		for (auto meshConf : meshConfigs) {
			auto mesh_visual = new MeshVisual(meshConf.MeshFileName,
					meshConf.TextureName);
			mesh_visual->getData().Visible = true;
			auto prepId_mesh = m_engines.render.addToPrepareVisual(mesh_visual);

			entity->addVisualPlaceholder(prepId_mesh);
		}

		if (collisionShape.isValid()) {
			CollisionHandler::setup(m_engines, *entity.get(),
					collisionShape.get());
		}

		logging::Info() << "Entity with " << meshConfigs.size()
				<< " mesh(es) created";
		return entity;
	}

private:

	Engines & m_engines;

};
