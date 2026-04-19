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
    
    // --- 個別設定監聽 ---
    
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

    // --- 全域設定監聽 ---
    // 修正：手動呼叫 SettingChangedEventV3 的構造函數
    // 傳入 "" 作為 key，代表不篩選特定設定，從而監聽該 Mod 的所有變動
    SettingChangedEvent(Mod::get(), "").listen([mm](auto setting) {
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
