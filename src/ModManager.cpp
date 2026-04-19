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
    
    // 個別設定監聽：當特定 Key 改變時更新 ModManager 的緩存值
    
    SettingChangedEvent(Mod::get(), "colour").listen([mm](auto setting) {
        mm->m_color1 = Mod::get()->getSettingValue<ccColor3B>("colour");
    }).leak();
    
    SettingChangedEvent(Mod::get(), "colour2").listen([mm](auto setting) {
        mm->m_color2 = Mod::get()->getSettingValue<ccColor3B>("colour2");
    }).leak();
    
    SettingChangedEvent(Mod::get(), "colour1Enabled").listen([mm](auto setting) {
        mm->m_color1Enabled = Mod::get()->getSettingValue<bool>("colour1Enabled");
    }).leak();
    
    SettingChangedEvent(Mod::get(), "colour2Enabled").listen([mm](auto setting) {
        mm->m_color2Enabled = Mod::get()->getSettingValue<bool>("colour2Enabled");
    }).leak();
    
    SettingChangedEvent(Mod::get(), "noWave").listen([mm](auto setting) {
        mm->m_noWave = Mod::get()->getSettingValue<bool>("noWave");
    }).leak();
    
    SettingChangedEvent(Mod::get(), "noDefaultTrail").listen([mm](auto setting) {
        mm->m_noDefaultTrail = Mod::get()->getSettingValue<bool>("noDefaultTrail");
    }).leak();
    
    SettingChangedEvent(Mod::get(), "solid").listen([mm](auto setting) {
        mm->m_solid = Mod::get()->getSettingValue<bool>("solid");
    }).leak();

    // 全域設定監聽：當該 Mod 的「任何」設定改變時，立即更新遊戲內的玩家拖尾效果
    // 注意：在 V3 中，SettingChangedEvent(Mod*) 是監聽全域變動的正確方式
    SettingChangedEvent(Mod::get()).listen([mm](auto setting) {
        if(!PlayLayer::get())
            return;

        auto p1 = static_cast<HookPlayerObject*>(PlayLayer::get()->m_player1);
        auto p2 = static_cast<HookPlayerObject*>(PlayLayer::get()->m_player2);

        if(p1 && p2) {
            p1->updateStreak();
            p2->updateStreak();
        }
    }).leak();
}
