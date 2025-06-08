#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "pokemon_game.h"

int main()
{
    InitWindow(GameWidth, GameHeight, "Pokemon Game");
    SetTargetFPS(60);

    int difficulty = 7;
    bool choosingDifficulty = true;
    float difficultySlider = 1.0f;

    while (!WindowShouldClose() && choosingDifficulty)
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            if (mousePos.y >= 300 && mousePos.y <= 330) {
                difficultySlider = (mousePos.x - 250) / 400.0f;
                if (difficultySlider < 0) difficultySlider = 0;
                if (difficultySlider > 1) difficultySlider = 1;
                difficulty = 1 + (int)(difficultySlider * 6);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Choose Difficulty", GetScreenWidth() / 2 - MeasureText("Choose Difficulty", 30) / 2, 200, 30, WHITE);
        DrawText("Use slider below to select difficulty (1-7)", GetScreenWidth() / 2 - MeasureText("Use slider below to select difficulty (1-7)", 20) / 2, 240, 20, LIGHTGRAY);

        DrawRectangle(250, 300, 400, 30, DARKGRAY);
        DrawRectangle(250, 300, (int)(400 * difficultySlider), 30, BLUE);
        DrawRectangleLines(250, 300, 400, 30, WHITE);
        DrawText(TextFormat("Difficulty: %d", difficulty), GetScreenWidth() / 2 - MeasureText(TextFormat("Difficulty: %d", difficulty), 20) / 2, 270, 20, WHITE);

        DrawText("Click to select difficulty, then press ENTER", GetScreenWidth() / 2 - MeasureText("Click to select difficulty, then press ENTER", 20) / 2, 350, 20, WHITE);

        if (IsKeyPressed(KEY_ENTER)) {
            choosingDifficulty = false;
        }

        EndDrawing();
    }
        
    PokemonGame game(10, 10, 50, difficulty);
    while (!WindowShouldClose())
    {
        game.Update();
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}