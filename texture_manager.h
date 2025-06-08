#pragma once

#include <raylib.h>
#include "pokemon.h"

class TextureManager {
private:
    Texture2D firePokemonTexture;
    Texture2D waterPokemonTexture;
    Texture2D grassPokemonTexture;
    Texture2D normalPokemonTexture;
    Texture2D battleBackground;
    Texture2D healthBar;
    Texture2D healthBarBackground;
    Texture2D attackButton;
    Texture2D specialButton;
    Texture2D mapBackground;
    Texture2D playerTexture;
    Texture2D enemyTexture;
    Texture2D potionTexture;
    Texture2D wallTexture;

public:
    TextureManager();
    ~TextureManager();

    Texture2D GetPokemonTexture(PokemonType type);
    Texture2D GetBattleBackground() const { return battleBackground; }
    Texture2D GetHealthBar() const { return healthBar; }
    Texture2D GetHealthBarBackground() const { return healthBarBackground; }
    Texture2D GetAttackButton() const { return attackButton; }
    Texture2D GetSpecialButton() const { return specialButton; }
    Texture2D GetMapBackground() const { return mapBackground; }
    Texture2D GetPlayerTexture() const { return playerTexture; }
    Texture2D GetEnemyTexture() const { return enemyTexture; }
    Texture2D GetPotionTexture() const { return potionTexture; }
    Texture2D GetWallTexture() const { return wallTexture; }
};

