#pragma once


#include <raylib.h>
#include <string>
#include <vector>
#include <algorithm>

#define POKEMON_TEXTURE_SIZE 128
#define HEALTH_BAR_WIDTH 200
#define HEALTH_BAR_HEIGHT 20
#define expToLevelUp 100

enum class PokemonType {
    FIRE,
    WATER,
    GRASS,
    NORMAL
};

class TextureManager;

class Pokemon {
protected:
    std::string name;
    int attack;
    int hp;
    int maxHp;
    int defence;
    PokemonType type;
    int level;
    int exp;
    Texture2D texture;
    TextureManager* textureManager;

public:
    Pokemon(std::string name, int attack, int hp, int defence, PokemonType type, TextureManager* texManager);
    virtual ~Pokemon() {};

    virtual void AttackEnemy(Pokemon* enemy);
    virtual void SpecialAttack(Pokemon* enemy) = 0;
    void TakeDamage(int damage);
    bool isFainted() const;
    void DisplayStat(int x, int y) const;
    void GainExp(int amount);
    void LevelUp();
    void Heal();
    void Draw(int x, int y, int size) const;

    PokemonType getType() const { return type; }
    std::string getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getLevel() const { return level; }
    int getAttack() const { return attack; }
    int getDefence() const { return defence; }
};

class FirePokemon : public Pokemon {
public:
    FirePokemon(std::string name, int attack, int hp, int defence, TextureManager* texManager);
    void SpecialAttack(Pokemon* enemy) override;
};

class WaterPokemon : public Pokemon {
public:
    WaterPokemon(std::string name, int attack, int hp, int defence, TextureManager* texManager);
    void SpecialAttack(Pokemon* enemy) override;
};

class GrassPokemon : public Pokemon {
public:
    GrassPokemon(std::string name, int attack, int hp, int defence, TextureManager* texManager);
    void SpecialAttack(Pokemon* enemy) override;
};

