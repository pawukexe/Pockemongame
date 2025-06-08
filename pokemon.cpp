#include "pokemon.h"
#include "texture_manager.h"

Pokemon::Pokemon(std::string name, int attack, int hp, int defence, PokemonType type, TextureManager* texManager) :
    name(name), attack(attack), hp(hp), maxHp(hp), defence(defence), type(type),
    level(1), exp(0), textureManager(texManager) {
    texture = textureManager->GetPokemonTexture(type);
}

void Pokemon::AttackEnemy(Pokemon* enemy) {
    int damage = attack - enemy->defence / 2;
    if (damage < 1) damage = 1;
    enemy->TakeDamage(damage);
}

void Pokemon::TakeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Pokemon::isFainted() const {
    return hp <= 0;
}

void Pokemon::DisplayStat(int x, int y) const {
    std::string typeStr;
    switch (type) {
    case PokemonType::FIRE: typeStr = "Fire"; break;
    case PokemonType::WATER: typeStr = "Water"; break;
    case PokemonType::GRASS: typeStr = "Grass"; break;
    default: typeStr = "Normal"; break;
    }

    DrawTexturePro(textureManager->GetHealthBarBackground(),
        { 0, 0, (float)textureManager->GetHealthBarBackground().width, (float)textureManager->GetHealthBarBackground().height },
        { (float)x, (float)y, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT },
        { 0, 0 }, 0, WHITE);

    float healthPercentage = (float)hp / (float)maxHp;
    DrawTexturePro(textureManager->GetHealthBar(),
        { 0, 0, (float)(textureManager->GetHealthBar().width * healthPercentage), (float)textureManager->GetHealthBar().height },
        { (float)x, (float)y, HEALTH_BAR_WIDTH * healthPercentage, HEALTH_BAR_HEIGHT },
        { 0, 0 }, 0, WHITE);

    DrawText(TextFormat("%s Lvl.%d", name.c_str(), level), x, y - 30, 20, WHITE);
    DrawText(TextFormat("HP: %d/%d", hp, maxHp), x + HEALTH_BAR_WIDTH + 10, y, 20, WHITE);
}

void Pokemon::GainExp(int amount) {
    exp += amount;
    if (exp >= expToLevelUp) {
        LevelUp();
        exp -= expToLevelUp;
    }
}

void Pokemon::LevelUp() {
    level++;
    maxHp += 10;
    hp = maxHp;
    attack += 5;
    defence += 3;
}

void Pokemon::Heal() {
    hp = maxHp;
}

void Pokemon::Draw(int x, int y, int size) const {
    if (texture.id != 0) {
        DrawTexturePro(texture,
            { 0, 0, (float)texture.width, (float)texture.height },
            { (float)x, (float)y, (float)size, (float)size },
            { 0, 0 }, 0, WHITE);
    }
}

FirePokemon::FirePokemon(std::string name, int attack, int hp, int defence, TextureManager* texManager) :
    Pokemon(name, attack, hp, defence, PokemonType::FIRE, texManager) {}

void FirePokemon::SpecialAttack(Pokemon* enemy) {
    int damage = attack * 1.5 - enemy->getDefence() / 2;
    if (enemy->getType() == PokemonType::GRASS) damage *= 2;
    if (damage < 1) damage = 1;
    enemy->TakeDamage(damage);
}

WaterPokemon::WaterPokemon(std::string name, int attack, int hp, int defence, TextureManager* texManager) :
    Pokemon(name, attack, hp, defence, PokemonType::WATER, texManager) {}

void WaterPokemon::SpecialAttack(Pokemon* enemy) {
    int damage = attack * 1.5 - enemy->getDefence() / 2;
    if (enemy->getType() == PokemonType::FIRE) damage *= 2;
    if (damage < 1) damage = 1;
    enemy->TakeDamage(damage);
}

GrassPokemon::GrassPokemon(std::string name, int attack, int hp, int defence, TextureManager* texManager) :
    Pokemon(name, attack, hp, defence, PokemonType::GRASS, texManager) {}

void GrassPokemon::SpecialAttack(Pokemon* enemy) {
    int damage = attack * 1.5 - enemy->getDefence() / 2;
    if (enemy->getType() == PokemonType::WATER) damage *= 2;
    if (damage < 1) damage = 1;
    enemy->TakeDamage(damage);
}