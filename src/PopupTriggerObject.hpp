#include <Geode/binding/SetupTriggerPopup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class $object(PopupTriggerObject, CustomTriggerObject) {
public:
    FLAlertLayer* popup;

    std::string title;
	std::string description;

	void setupCustomObject() override {
		setupObjectProperty(343, title);
		setupObjectProperty(344, description);
	} // setupCustomObject

    void resetCustomObject() override {
        if (popup) popup->onBtn1(this); // Close popup
    } // resetCustomObject

	void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) override {
        if (popup) return; // Don't open another popup if one already exists

        std::string titleStr = title.length() > 0 ? title : "Title";
        std::string descStr = description.length() > 0 ? description : "Description";

        auto callback = [this](auto, bool) { popup = nullptr; };
        popup = geode::createQuickPopup(titleStr.c_str(), descStr, "Ok", nullptr, callback);
    } // activateCustomTrigger
};

class SetupPopupTriggerPopup : public SetupTriggerPopup {
public:
    std::string title;
    std::string description;

    static SetupPopupTriggerPopup* create(PopupTriggerObject* obj, cocos2d::CCArray* objs) {
        auto popup = new SetupPopupTriggerPopup();
        if (popup->init(obj, objs)) {
            popup->autorelease();
            return popup;
        } // if

        delete popup;
        return nullptr;
    } // create

    bool init(PopupTriggerObject* obj, cocos2d::CCArray* objs) {
        if (!SetupTriggerPopup::init(obj, objs, 300, 220, 1)) return false;
        preSetup();

        auto winSize = CCDirector::get()->getWinSize();

        addTitle("Setup Popup Trigger");
        addHelpButton("Popup Trigger Help", "This trigger opens a custom popup wow!", 0.9);
        createMultiTriggerItemsDefault();
        addCloseButton("OK");

        auto titleInput = TextInput::create(240, "Title");
        titleInput->setPosition(CCPoint(winSize.width / 2, winSize.height / 2 + 40));
        titleInput->setCommonFilter(CommonFilter::Any);
        titleInput->setMaxCharCount(20);
        m_mainLayer->addChild(titleInput);

        auto descInput = TextInput::create(240, "Description");
        descInput->setPosition(CCPoint(winSize.width / 2, winSize.height / 2 - 5));
        descInput->setCommonFilter(CommonFilter::Any);
        descInput->setMaxCharCount(100);
        m_mainLayer->addChild(descInput);

        titleInput->setCallback([this](auto str) { title = str; });
        descInput->setCallback([this](auto str) { description = str; });

        auto trigger = obj ? obj : static_cast<PopupTriggerObject*>(objs->firstObject());
        titleInput->setString(trigger->title, true);
        descInput->setString(trigger->description, true);

        postSetup();
        return true;
    } // init

    void onClose(cocos2d::CCObject* sender) override {
        for (auto obj : getObjects()->asExt()) {
            auto trigger = static_cast<PopupTriggerObject*>(obj);
            trigger->title = title;
            trigger->description = description;
        } // for
        SetupTriggerPopup::onClose(sender);
    } // onClose
};