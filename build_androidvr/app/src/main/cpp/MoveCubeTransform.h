#pragma once

#include <SpheresEngine/InputEngine/InputTransformer.h>
#include <SpheresEngine/Util.h>

class MoveCubeAction final: public InputAction {
public:
    /*
     * The rotation speed is indicated by speed parameter which is [-1,1]
     */
    MoveCubeAction(Quaternion orientation, float speed = 0.0f) :
            Orientation(orientation), Speed(speed) {

    }
    virtual ~MoveCubeAction() = default;

    Quaternion Orientation;
    float Speed = 0.0f;
};

class ChangeColorAction final: public InputAction {
};

class MoveCubeTransform: public InputTransformer {
public:
    virtual ~MoveCubeTransform() = default;

    std::unique_ptr<InputAction> transform(Input ip) override {
        if ((ip.type() == Input::Type::Orientation) && (ip.device() == Input::Device::MagicWand)) {
                return std14::make_unique<MoveCubeAction>(ip.orientation());
        }

        // nothing to report
        return nullptr;
    }

};


class ChangeColorTransform: public InputTransformer {
public:
    virtual ~ChangeColorTransform() = default;

    std::unique_ptr<InputAction> transform(Input ip) override {
        if ((ip.type() == Input::Type::ButtonDown) && (ip.device() == Input::Device::MagicWand)) {
            return std14::make_unique<ChangeColorAction>();
        }

        // nothing to report
        return nullptr;
    }

};