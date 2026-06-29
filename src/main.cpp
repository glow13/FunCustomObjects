#include <glow12.custom-objects-api/include/CustomObjectsAPI.hpp>

#include <glow12.custom-objects-api/objects/CustomTriggerObject.hpp>
#include <glow12.custom-objects-api/objects/CustomRingObject.hpp>
#include <glow12.custom-objects-api/objects/CustomPadObject.hpp>
#include <glow12.custom-objects-api/objects/CustomPortalObject.hpp>
#include <glow12.custom-objects-api/objects/CustomRotateObject.hpp>
#include <glow12.custom-objects-api/objects/CustomCollectibleObject.hpp>
#include <glow12.custom-objects-api/objects/CustomAnimatedObject.hpp>

#include <Geode/binding/SetupCameraModePopup.hpp>

#include "PopupTriggerObject.hpp"

using namespace geode::prelude;

class $object(MyCustomPad, CustomPadObject) {
    float bouncePower = 0.4f;

    void resetCustomObject() override {
        bouncePower = 0.4f;
    } // resetCustomObject

    void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) override {
        bumpPlayer(player, bouncePower, GameObjectType::RedJumpPad);
        bouncePower += 0.1f;
    } // activateCustomObject
};

class $object(DecayBlock, CustomGameObject) {
    int decay;

    void setupCustomObject() override {
        decay = rand() % 10;
    } // setupCustomObject

    void resetCustomObject() override {
        if (decay < 0 && !m_editorEnabled) {
            GJBaseGameLayer::get()->destroyObject(this);
        } else decay--;
    } // resetCustomObject
};

$execute {
    CustomObjectsAPI::registerCustomObject<CustomPadObject>("smile-block.png"_spr).setBoxSize(30, 30).setParticleColor(0, 0, 0)
        .onActivateCustomObject([](CustomPadObject* obj, auto, PlayerObject* player) {
            player->setPosition(obj->getPosition() + CCPoint(0, (rand() % 9 + 2) * 30));
            player->setYVelocity(1, 1);
        });

    CustomObjectsAPI::registerCustomObject("frown-block.png"_spr).setBoxSize(20, 20).setObjectType(GameObjectType::Hazard);
    CustomObjectsAPI::registerCustomObject<MyCustomPad>("bump_02_001.png").setGlowSprite("bump_02_glow_001.png")
        .setGlowColor(255, 100, 100).setParticleColor(255, 0, 0).setObjectOffset(0, -12).setEditorTabPriority(1).setBoxSize(29, 7);

    CustomObjectsAPI::registerCustomObject("block-1.png"_spr);
    CustomObjectsAPI::registerCustomObject("block-2.png"_spr);
    CustomObjectsAPI::registerCustomObject("block-3.png"_spr);
    CustomObjectsAPI::registerCustomObject("block-4.png"_spr);
    CustomObjectsAPI::registerCustomObject("block-5.png"_spr);
    CustomObjectsAPI::registerCustomObject("block-6.png"_spr);
    CustomObjectsAPI::registerCustomObject("block-7.png"_spr, 60, 30);
    CustomObjectsAPI::registerCustomObject("block-8.png"_spr, 60, 30);
    CustomObjectsAPI::registerCustomObject("block-9.png"_spr, 60, 30);
    CustomObjectsAPI::registerCustomObject("block-10.png"_spr, 60);
    CustomObjectsAPI::registerCustomObject("block-11.png"_spr, 60);
    CustomObjectsAPI::registerCustomObject("block-12.png"_spr, 60);

    CustomObjectsAPI::registerCustomObject("spike_01_001.png").setGlowSprite("spike_01_glow_001.png").setBoxSize(5, 24).setObjectType(GameObjectType::Hazard);
    CustomObjectsAPI::registerCustomObject("block005_02_001.png", 60).setDetailSprite("block005_02_color_001.png", 60).setObjectType(GameObjectType::Decoration);

    CustomObjectsAPI::registerCustomObject<CustomPadObject>("bump_03_001.png").setGlowSprite("bump_03_glow_001.png")
        .setGlowColor(255, 0, 255).setParticleColor(255, 0, 255).setObjectOffset(0, -13).setBatchMode(0).setEditorTabPriority(1)
        .onActivateCustomObject([](CustomPadObject* obj, auto level, auto player) {
            obj->bumpPlayer(player, 0.65f, GameObjectType::PinkJumpPad);
            if (rand() % 50 == 0) level->destroyPlayer(player, obj);
        });

    CustomObjectsAPI::registerCustomObject<CustomRotateObject>("blade_02_001.png").setGlowSprite("blade_02_glow_001.png").setBoxRadius(22).setObjectType(GameObjectType::Hazard).setBatchMode(0);
    CustomObjectsAPI::registerCustomObject<CustomPortalObject>("portal_18_front_001.png").setDetailSprite("portal_18_back_001.png").setParticleColor(255, 255, 0).setBatchMode(1)
        .onEditObjectButton([](auto obj, auto objs) {
            SetupCameraModePopup::create(obj, objs)->show();
        })
        .onActivateCustomObject([](CustomPortalObject* obj, auto level, auto player) {
            GameModeType type;
            switch (rand() % 6) {
                case 0: type = GameModeType::SHIP; break;
                case 1: type = GameModeType::CUBE; break;
                case 2: type = GameModeType::BALL; break;
                case 3: type = GameModeType::UFO; break;
                case 4: type = GameModeType::ROBOT; break;
                case 5: type = GameModeType::SPIDER; break;
                default: type = GameModeType::CUBE; break;
            } // switch

            obj->switchPlayerMode(level, player, type);
            obj->playShineEffect((GameObjectType)type);
        });

    CustomObjectsAPI::registerCustomObject<CustomCollectibleObject>("d_key01_001.png").setDetailSprite("d_key01_color_001.png").setBatchMode(0)
        .onActivateCustomObject([](auto, auto, auto) {
            log::info("COLLECTED ME!!!!!!!");
        });

    CustomObjectsAPI::registerCustomObject<CustomAnimatedObject>("cat_001.png"_spr).setFramesCount(94).setFrameTime(0.03).setEditorTabPriority(-1);
    CustomObjectsAPI::registerCustomAnimationSprites("cat_001.png"_spr, 94);

	CustomObjectsAPI::registerCustomObject<PopupTriggerObject>("popup_trigger.png"_spr, 28, 31).setEditorTabPriority(-10)
		.onEditObjectButton([](auto obj, auto objs) { SetupPopupTriggerPopup::create(obj, objs)->show(); });

    CustomObjectsAPI::registerCustomObject<DecayBlock>("square_01_001.png").setGlowSprite("blockOutline_01_glow_001.png").setObjectType(GameObjectType::Solid);
}