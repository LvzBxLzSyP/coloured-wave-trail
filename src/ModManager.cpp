#include "ModManager.hpp"
#include "PlayerObject.hpp"

using namespace geode::prelude;

ModManager* ModManager::sharedState() {
    static ModManager instance;
    return &instance;
}

void ModManager::load() {
    auto mod = Mod::get();
    
    m_color1 = mod->getSettingValue<ccColor3B>("colour");
    m_color2 = mod->getSettingValue<ccColor3B>("colour2");
    m_color1Enabled = mod->getSettingValue<bool>("colour1Enabled");
    m_color2Enabled = mod->getSettingValue<bool>("colour2Enabled");
    m_noWave = mod->getSettingValue<bool>("noWave");
    m_noDefaultTrail = mod->getSettingValue<bool>("noDefaultTrail");
    m_solid = mod->getSettingValue<bool>("solid");

}

$on_mod(Loaded) {
    auto mm = ModManager::sharedState();
    mm->load();
    
    SettingChangedEvent("colour").listen([mm](ccColor3B val) {
        mm->m_color1 = val;
    }).leak();
    
    SettingChangedEvent("colour2").listen([mm](ccColor3B val) {
        mm->m_color2 = val;
    }).leak();
    
    SettingChangedEvent("colour1Enabled").listen([mm](bool val) {
        mm->m_color1Enabled = val;
    }).leak();
    
    SettingChangedEvent("colour2Enabled").listen([mm](bool val) {
        mm->m_color2Enabled = val;
    }).leak();
    
    SettingChangedEvent("noWave").listen([mm](bool val) {
        mm->m_noWave = val;
    }).leak();
    
    SettingChangedEvent("noDefaultTrail").listen([mm](bool val) {
        mm->m_noDefaultTrail = val;
    }).leak();
    
    SettingChangedEvent("solid").listen([mm](bool val) {
        mm->m_solid = val;
    }).leak();

    listenForAllSettingChanges([](std::shared_ptr<SettingV3> setting) {
        if(!PlayLayer::get())
            return;

        auto p1 = static_cast<HookPlayerObject*>(PlayLayer::get()->m_player1);
        auto p2 = static_cast<HookPlayerObject*>(PlayLayer::get()->m_player2);

        if(p1 && p2) {
            p1->updateStreak();
            p2->updateStreak();
        }
    });
}
