#include "texture_manager.h"

TextureManager::TextureManager() {
    firePokemonTexture = LoadTexture("resources/fire_pokemon.png");
    waterPokemonTexture = LoadTexture("resources/water_pokemon.png");
    grassPokemonTexture = LoadTexture("resources/grass_pokemon.png");
    normalPokemonTexture = LoadTexture("resources/normal_pokemon.png");
    battleBackground = LoadTexture("resources/battle_bg.png");
    healthBar = LoadTexture("resources/health_bar.png");
    healthBarBackground = LoadTexture("resources/health_bar_bg.png");
    attackButton = LoadTexture("resources/attack_button.png");
    specialButton = LoadTexture("resources/special_button.png");
    mapBackground = LoadTexture("resources/map_bg.png");
    playerTexture = LoadTexture("resources/player.png");
    enemyTexture = LoadTexture("resources/enemy.png");
    potionTexture = LoadTexture("resources/potion.png");
    wallTexture = LoadTexture("resources/wall.png");
}

TextureManager::~TextureManager() {
    UnloadTexture(firePokemonTexture);
    UnloadTexture(waterPokemonTexture);
    UnloadTexture(grassPokemonTexture);
    UnloadTexture(normalPokemonTexture);
    UnloadTexture(battleBackground);
    UnloadTexture(healthBar);
    UnloadTexture(healthBarBackground);
    UnloadTexture(attackButton);
    UnloadTexture(specialButton);
    UnloadTexture(mapBackground);
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(potionTexture);
    UnloadTexture(wallTexture);
}

Texture2D TextureManager::GetPokemonTexture(PokemonType type) {
    switch (type) {
    case PokemonType::FIRE: return firePokemonTexture;
    case PokemonType::WATER: return waterPokemonTexture;
    case PokemonType::GRASS: return grassPokemonTexture;
    default: return normalPokemonTexture;
    }
}