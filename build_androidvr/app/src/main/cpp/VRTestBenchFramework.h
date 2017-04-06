#pragma once

#include <SpheresEngine/AndroidVRFramework.h>

#include <SpheresEngine/RenderEngine/RenderBackendAndroidVR.h>

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_audio.h>
#include <vr/gvr/capi/include/gvr_types.h>

#include <memory>
#include <random>

#include "MoveCubeTransform.h"

class MoveCubeAspect: public Aspect<PositionedEntity> {
public:
    MoveCubeAspect( InputEngine & ie) : m_ie(ie) {

    }

    void init(PositionedEntity * ent) override {
        m_ie.OnNewInputAction.subscribe(
                [ent, this]( InputAction * ia )
                {
                    auto inpAction = dynamic_cast< MoveCubeAction * >(ia);
                    if (inpAction) {
                        ent->setRotation( inpAction->Orientation );
                    }

                    auto inpActionChangeColor = dynamic_cast< ChangeColorAction * >(ia);
                    if (inpActionChangeColor) {
                        // cycle the texture fo the box
                        for (auto & v : ent->getVisuals() ) {
                            MeshVisual *vent = static_cast<MeshVisual *> (v);
                            vent->getData().Visible = !vent->getData().Visible;
                        }
                    }
                });
    }

private:
    InputEngine & m_ie;
};

class VRTestBenchFramework final: public AndroidVRFramework {
public:
	VRTestBenchFramework(gvr_context* gvr_context,
			std::unique_ptr<gvr::AudioApi> gvr_audio_api) :
	AndroidVRFramework(gvr_context,std::move(gvr_audio_api)) {

	}

	~VRTestBenchFramework() = default;

	virtual void initContent() override {

        m_inputEngine.addTransformer(std14::make_unique<MoveCubeTransform>());
        m_inputEngine.addTransformer(std14::make_unique<ChangeColorTransform>());

        for (size_t i = 0; i < 40; i++) {
            addRotatingCube(m_entityEngine, *m_re.get(), m_animationEngine, m_inputEngine);
        }
    }


    void addRotatingCube(EntityEngine & ee, RenderEngine & re,
                         AnimationEngine & ae, InputEngine & ie) {

        std::uniform_real_distribution<float> distribution_x(4.0, 10.0);
        std::uniform_real_distribution<float> distribution_speed(0.02, 0.1);
        std::uniform_int_distribution<int> distribution_rot_dir(0,2);

        // add a yellow and red box so the user can switch between them
        auto mv_yellow = new MeshVisual("debug_box", "debug_texture");
        mv_yellow->getData().Center = glm::vec3(0, 0, 0.5);
        mv_yellow->getData().Visible = false;
        auto prepId_yellow = re.addToPrepareVisual(mv_yellow);

        auto mv_red = new MeshVisual("debug_box", "debug_texture_red");
        mv_red->getData().Center = glm::vec3(0, 0, 0.5);
        mv_red->getData().Visible = true;
        auto prepId_red = re.addToPrepareVisual(mv_red);


        auto boxEntity = std14::make_unique<PositionedEntity>();
        auto boxEntityPtr = boxEntity.get();
        boxEntity->addVisualPlaceholder(prepId_yellow);
        boxEntity->addVisualPlaceholder(prepId_red);
        boxEntity->addAspect(std14::make_unique<MoveCubeAspect>(ie));
        //boxEntity->addAspect(std14::make_unique<RotateCubeAspect>(ee, ie));
        ee.addEntity(std::move(boxEntity));

        auto rot_length = distribution_x(m_generator);
        auto rot_dir = distribution_rot_dir(m_generator);
        auto rot_speed = distribution_speed(m_generator);

        // add animation to rotate the galaxy
        auto lmbRotateCube =
                [boxEntityPtr, rot_length, rot_speed, rot_dir](float cur, float end) {

                    float rot_x = 0.0f;
                    float rot_y = 0.0f;
                    float rot_z = 0.0f;

                    Vector3 rotateAround;
                    if (rot_dir == 0) {
                        rotateAround = Vector3(0, 1, 0);
                        rot_x = rot_length;
                    }
                    if (rot_dir == 1) {
                        rotateAround = Vector3(1, 0, 0);
                        rot_y = rot_length;
                    }
                    if (rot_dir == 2) {
                        rotateAround = Vector3(0, 0, 1);
                        rot_x = rot_length;
                    }

                    // rotate position around center
                    boxEntityPtr->rotateAround( Vector3(0,0,0),Vector3( rot_x , rot_y, rot_z), rotateAround,
                                                rot_speed *
                                                (cur / end)
                                                * 2.0f * boost::math::constants::pi<float>());

                };

        Animation an( { { 0.0f, Sequence(100.0f, lmbRotateCube) } }, true);
        ae.addAnimation(an, 0.5f);

    }

private:
    std::default_random_engine m_generator;

};
