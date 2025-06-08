#pragma once

#include <vector>
#include <raylib.h>
#include "pokemon.h"
#include "game_map.h"
#include "texture_manager.h"

#define BATTLE_BG_WIDTH 800
#define BATTLE_BG_HEIGHT 600
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define GameWidth 900
#define GameHeight 600

class PokemonGame {
private:
    std::vector<Pokemon*> playerTeam;
    std::vector<Pokemon*> enemyPokemons;
    GameMap map;
    int playerX, playerY;
    bool inBattle;
    Pokemon* currentEnemy;
    int selectedPokemon;
    int battleAction;
    bool gameOver;
    bool victory;
    bool inSelectionScreen;
    std::vector<Pokemon*> availablePokemons;
    std::vector<bool> selectedPokemons;
    int selectionCursor;
    TextureManager textureManager;
    bool canSwitchPokemon;

    void InitializePokemons();
    void CheckWinLoseConditions();
    void HandleSelectionInput();
    void FinalizeTeamSelection();
    void HandleGameEndInput();
    void RestartGame(bool newTeam);
    void HandleMovement();
    void MovePlayer(int dx, int dy);
    void StartBattle();
    void HandleBattleInput();
    void ExecuteBattleAction();
    void DrawSelectionScreen() const;
    void DrawHUD() const;
    void DrawBattleScreen() const;
    void DrawGameOverScreen() const;
    void DrawVictoryScreen() const;

public:
    PokemonGame(int w, int h, int cs, int diff);
    ~PokemonGame();
    void Update();
    void Draw();
};

