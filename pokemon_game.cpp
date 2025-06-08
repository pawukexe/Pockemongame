#include "pokemon_game.h"

PokemonGame::PokemonGame(int w, int h, int cs, int diff) :
    map(w, h, cs, diff, &textureManager), textureManager() {
    playerX = 0;
    playerY = 0;
    inBattle = false;
    gameOver = false;
    victory = false;
    selectedPokemon = 0;
    battleAction = 0;
    inSelectionScreen = true;
    selectionCursor = 0;
    currentEnemy = nullptr;
    canSwitchPokemon = true;

    availablePokemons.push_back(new FirePokemon("Blazewing", 15, 50, 10, &textureManager));
    availablePokemons.push_back(new WaterPokemon("Tidepaw", 12, 60, 12, &textureManager));
    availablePokemons.push_back(new GrassPokemon("Vineclaw", 10, 70, 15, &textureManager));
    availablePokemons.push_back(new FirePokemon("Pyroclaw", 14, 45, 8, &textureManager));
    availablePokemons.push_back(new WaterPokemon("Mistral", 13, 65, 9, &textureManager));
    availablePokemons.push_back(new GrassPokemon("Thornspout", 11, 55, 12, &textureManager));
    availablePokemons.push_back(new FirePokemon("Inferno", 16, 55, 11, &textureManager));
    selectedPokemons.resize(availablePokemons.size(), false);
}

PokemonGame::~PokemonGame() {
    for (auto p : playerTeam) delete p;
    for (auto p : enemyPokemons) delete p;
    for (auto p : availablePokemons) delete p;
}

void PokemonGame::InitializePokemons() {
    for (int i = 0; i < 5 + map.GetDifficulty() * 2; i++) {
        int type = GetRandomValue(0, 2);
        switch (type) {
        case 0:
            enemyPokemons.push_back(new FirePokemon("Ponyta", GetRandomValue(7, 15), GetRandomValue(35, 80), GetRandomValue(6, 10), &textureManager));
            break;
        case 1:
            enemyPokemons.push_back(new WaterPokemon("Poliwag", GetRandomValue(8, 20), GetRandomValue(45, 75), GetRandomValue(5, 12), &textureManager));
            break;
        case 2:
            enemyPokemons.push_back(new GrassPokemon("Oddish", GetRandomValue(10, 17), GetRandomValue(40, 90), GetRandomValue(8, 15), &textureManager));
            break;
        }
    }
}

void PokemonGame::CheckWinLoseConditions() {
    bool allFainted = true;
    for (auto p : playerTeam) {
        if (!p->isFainted()) {
            allFainted = false;
            break;
        }
    }

    if (allFainted) {
        gameOver = true;
        inBattle = false;
        return;
    }

    if (enemyPokemons.empty() && !inBattle) {
        victory = true;
    }
}

void PokemonGame::Update() {
    if (inSelectionScreen) {
        HandleSelectionInput();
        return;
    }
    if (gameOver || victory) {
        HandleGameEndInput();
        return;
    }
    CheckWinLoseConditions();
    if (!inBattle) {
        HandleMovement();
    }
    else {
        HandleBattleInput();
    }
}

void PokemonGame::HandleSelectionInput() {
    if (IsKeyPressed(KEY_UP)) {
        selectionCursor = (selectionCursor - 1 + availablePokemons.size()) % availablePokemons.size();
    }
    else if (IsKeyPressed(KEY_DOWN)) {
        selectionCursor = (selectionCursor + 1) % availablePokemons.size();
    }
    else if (IsKeyPressed(KEY_ENTER)) {
        selectedPokemons[selectionCursor] = !selectedPokemons[selectionCursor];
        int selectedCount = 0;
        for (bool selected : selectedPokemons) {
            if (selected) selectedCount++;
        }
        if (selectedCount >= 3) {
            FinalizeTeamSelection();
        }
    }
}

void PokemonGame::FinalizeTeamSelection() {
    for (int i = 0; i < availablePokemons.size(); i++) {
        if (selectedPokemons[i] && playerTeam.size() < 3) {
            Pokemon* original = availablePokemons[i];
            switch (original->getType()) {
            case PokemonType::FIRE:
                playerTeam.push_back(new FirePokemon(original->getName(), original->getAttack(),
                    original->getMaxHp(), original->getDefence(), &textureManager));
                break;
            case PokemonType::WATER:
                playerTeam.push_back(new WaterPokemon(original->getName(), original->getAttack(),
                    original->getMaxHp(), original->getDefence(), &textureManager));
                break;
            case PokemonType::GRASS:
                playerTeam.push_back(new GrassPokemon(original->getName(), original->getAttack(),
                    original->getMaxHp(), original->getDefence(), &textureManager));
                break;
            }
        }
    }
    InitializePokemons();
    inSelectionScreen = false;
}

void PokemonGame::HandleGameEndInput() {
    if (IsKeyPressed(KEY_R)) RestartGame(false);
    else if (IsKeyPressed(KEY_N)) RestartGame(true);
}

void PokemonGame::RestartGame(bool newTeam) {
    for (auto p : enemyPokemons) delete p;
    enemyPokemons.clear();
    playerX = 0;
    playerY = 0;
    inBattle = false;
    gameOver = false;
    victory = false;
    selectedPokemon = 0;
    battleAction = 0;
    canSwitchPokemon = true;
    map.GenerateMap();
    if (newTeam) {
        for (auto p : playerTeam) delete p;
        playerTeam.clear();
        std::fill(selectedPokemons.begin(), selectedPokemons.end(), false);
        selectionCursor = 0;
        inSelectionScreen = true;
    }
    else {
        for (auto p : playerTeam) p->Heal();
        InitializePokemons();
    }
}

void PokemonGame::HandleMovement() {
    if (IsKeyPressed(KEY_W)) MovePlayer(0, -1);
    else if (IsKeyPressed(KEY_S)) MovePlayer(0, 1);
    else if (IsKeyPressed(KEY_A)) MovePlayer(-1, 0);
    else if (IsKeyPressed(KEY_D)) MovePlayer(1, 0);
}

void PokemonGame::MovePlayer(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;
    char cell = map.GetCell(newX, newY);
    if (cell == 'W') return;
    playerX = newX;
    playerY = newY;
    if (cell == 'E') StartBattle();
    else if (cell == 'P') {
        for (auto p : playerTeam) p->Heal();
        map.ClearCell(playerX, playerY);
    }
}

void PokemonGame::StartBattle() {
    if (enemyPokemons.empty()) return;
    currentEnemy = enemyPokemons.back();
    enemyPokemons.pop_back();
    inBattle = true;
    selectedPokemon = 0;
    battleAction = 0;
    canSwitchPokemon = true;
    map.ClearCell(playerX, playerY);
}

void PokemonGame::HandleBattleInput() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        Rectangle attackButton = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() - 100, 200, 40 };
        if (CheckCollisionPointRec(mousePos, attackButton)) {
            if (!playerTeam[selectedPokemon]->isFainted()) {
                battleAction = 0;
                ExecuteBattleAction();
            }
            return;
        }

        Rectangle specialButton = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() - 50, 200, 40 };
        if (CheckCollisionPointRec(mousePos, specialButton)) {
            if (!playerTeam[selectedPokemon]->isFainted()) {
                battleAction = 1;
                ExecuteBattleAction();
            }
            return;
        }
    }

    if (playerTeam[selectedPokemon]->isFainted()) {
        if (IsKeyPressed(KEY_UP)) {
            selectedPokemon = (selectedPokemon - 1 + playerTeam.size()) % playerTeam.size();
        }
        else if (IsKeyPressed(KEY_DOWN)) {
            selectedPokemon = (selectedPokemon + 1) % playerTeam.size();
        }
    }
}

void PokemonGame::ExecuteBattleAction() {
    if (playerTeam[selectedPokemon]->isFainted()) return;

    if (battleAction == 0) {
        playerTeam[selectedPokemon]->AttackEnemy(currentEnemy);
    }
    else {
        playerTeam[selectedPokemon]->SpecialAttack(currentEnemy);
    }

    if (currentEnemy->isFainted()) {
        for (auto p : playerTeam) {
            if (!p->isFainted()) p->GainExp(30);
        }
        delete currentEnemy;
        inBattle = false;
        CheckWinLoseConditions();
        return;
    }

    if (GetRandomValue(0, 1)) {
        currentEnemy->AttackEnemy(playerTeam[selectedPokemon]);
    }
    else {
        currentEnemy->SpecialAttack(playerTeam[selectedPokemon]);
    }

    CheckWinLoseConditions();
}

void PokemonGame::Draw() {
    if (inSelectionScreen) {
        DrawSelectionScreen();
        return;
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int mapOffsetX = 50;
    int mapOffsetY = 50;

    map.Draw(mapOffsetX, mapOffsetY);

    DrawTexturePro(textureManager.GetPlayerTexture(),
        { 0, 0, (float)textureManager.GetPlayerTexture().width, (float)textureManager.GetPlayerTexture().height },
        { (float)(mapOffsetX + playerX * map.GetCellSize()),
          (float)(mapOffsetY + playerY * map.GetCellSize()),
          (float)map.GetCellSize(), (float)map.GetCellSize() },
        { 0, 0 }, 0, WHITE);

    if (inBattle) DrawBattleScreen();
    else if (gameOver) DrawGameOverScreen();
    else if (victory) DrawVictoryScreen();
    else DrawHUD();
}

void PokemonGame::DrawSelectionScreen() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    DrawText("Choose your 3 Pokemon team:", 100, 50, 30, WHITE);

    for (int i = 0; i < availablePokemons.size(); i++) {
        Color textColor = WHITE;
        if (i == selectionCursor) textColor = YELLOW;
        if (selectedPokemons[i]) textColor = GREEN;

        std::string pokemonInfo = availablePokemons[i]->getName() + " (Lvl. 1)";
        DrawText(pokemonInfo.c_str(), 100, 100 + i * 40, 25, textColor);

        if (i == selectionCursor) {
            availablePokemons[i]->Draw(400, 100, POKEMON_TEXTURE_SIZE);
            availablePokemons[i]->DisplayStat(400, 140 + POKEMON_TEXTURE_SIZE);
        }
    }

    DrawText("Use UP/DOWN to navigate, ENTER to select", 100, 400, 20, WHITE);
    int selectedCount = 0;
    for (bool selected : selectedPokemons) if (selected) selectedCount++;
    DrawText(TextFormat("Selected: %d/3", selectedCount), 100, 430, 20, WHITE);
}

void PokemonGame::DrawHUD() const {
    DrawText("Controls: WASD - movement. Cages: red - enemies, green - heal.", 50, 10, 20, WHITE);

    for (int i = 0; i < playerTeam.size(); i++) {
        playerTeam[i]->Draw(575, 50 + i * 180, POKEMON_TEXTURE_SIZE / 2);
        playerTeam[i]->DisplayStat(575, 50 + i * 180 + POKEMON_TEXTURE_SIZE / 2 + 40);

        if (playerTeam[i]->isFainted()) {
            DrawText("FAINTED", 575, 50 + i * 180 + POKEMON_TEXTURE_SIZE / 2 + 70, 20, RED);
        }
    }

    DrawText(TextFormat("Enemies left: %d", enemyPokemons.size()), 50, 560, 20, WHITE);
}

void PokemonGame::DrawBattleScreen() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 200 });

    DrawTexturePro(textureManager.GetBattleBackground(),
        { 0, 0, (float)textureManager.GetBattleBackground().width, (float)textureManager.GetBattleBackground().height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 }, 0, WHITE);

    const int enemyX = GetScreenWidth() - 325;
    const int enemyY = 75;
    const int playerX = 100;
    const int playerY = GetScreenHeight() - 200;

    currentEnemy->Draw(enemyX, enemyY, POKEMON_TEXTURE_SIZE * 2);

    currentEnemy->DisplayStat(enemyX, enemyY + 140 + POKEMON_TEXTURE_SIZE);

    playerTeam[selectedPokemon]->Draw(playerX, playerY, POKEMON_TEXTURE_SIZE * 2);

    playerTeam[selectedPokemon]->DisplayStat(playerX + 20, playerY - 30);

    const int buttonY = GetScreenHeight() - 100;

    Color attackColor = (battleAction == 0) ? YELLOW : WHITE;
    DrawRectangle(GetScreenWidth() / 2 - 100, buttonY, 200, 40, DARKGRAY);
    DrawText("ATTACK", GetScreenWidth() / 2 - MeasureText("ATTACK", 30) / 2, buttonY + 5, 30, attackColor);
    DrawRectangleLines(GetScreenWidth() / 2 - 100, buttonY, 200, 40, WHITE);

    Color specialColor = (battleAction == 1) ? YELLOW : WHITE;
    DrawRectangle(GetScreenWidth() / 2 - 100, buttonY + 50, 200, 40, DARKGRAY);
    DrawText("SPECIAL", GetScreenWidth() / 2 - MeasureText("SPECIAL", 30) / 2, buttonY + 55, 30, specialColor);
    DrawRectangleLines(GetScreenWidth() / 2 - 100, buttonY + 50, 200, 40, WHITE);

    if (playerTeam[selectedPokemon]->isFainted()) {
        DrawText("POKEMON FAINTED!", playerX + POKEMON_TEXTURE_SIZE - MeasureText("POKEMON FAINTED!", 20) / 2, buttonY - 230, 20, RED);
        DrawText("SELECT ANOTHER WITH UP/DOWN KEYS!", playerX + POKEMON_TEXTURE_SIZE - MeasureText("SELECT ANOTHER WITH UP/DOWN KEYS!", 20) / 2, buttonY - 200, 20, RED);
    }
}

void PokemonGame::DrawGameOverScreen() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 200 });
    DrawText("Game Over!", GetScreenWidth() / 2 - MeasureText("Game Over!", 40) / 2,
        GetScreenHeight() / 2 - 80, 40, RED);
    DrawText("All your pokemons have fainted", GetScreenWidth() / 2 - MeasureText("All your pokemons have fainted", 30) / 2,
        GetScreenHeight() / 2 - 30, 30, WHITE);
    DrawText("Press R to restart with same team", GetScreenWidth() / 2 - MeasureText("Press R to restart with same team", 20) / 2,
        GetScreenHeight() / 2 + 20, 20, WHITE);
    DrawText("Press N to choose new team", GetScreenWidth() / 2 - MeasureText("Press N to choose new team", 20) / 2,
        GetScreenHeight() / 2 + 50, 20, WHITE);
    DrawText("Press ESC to exit", GetScreenWidth() / 2 - MeasureText("Press ESC to exit", 20) / 2,
        GetScreenHeight() / 2 + 80, 20, WHITE);
}

void PokemonGame::DrawVictoryScreen() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 200 });
    DrawText("Victory!", GetScreenWidth() / 2 - MeasureText("Victory!", 40) / 2,
        GetScreenHeight() / 2 - 80, 40, GREEN);
    DrawText("You defeated all enemy pokemons!", GetScreenWidth() / 2 - MeasureText("You defeated all enemy pokemons!", 30) / 2,
        GetScreenHeight() / 2 - 30, 30, WHITE);
    DrawText("Press R to restart with same team", GetScreenWidth() / 2 - MeasureText("Press R to restart with same team", 20) / 2,
        GetScreenHeight() / 2 + 20, 20, WHITE);
    DrawText("Press N to choose new team", GetScreenWidth() / 2 - MeasureText("Press N to choose new team", 20) / 2,
        GetScreenHeight() / 2 + 50, 20, WHITE);
    DrawText("Press ESC to exit", GetScreenWidth() / 2 - MeasureText("Press ESC to exit", 20) / 2,
        GetScreenHeight() / 2 + 80, 20, WHITE);
}