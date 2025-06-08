#include "game_map.h"
#include <queue>
#include <algorithm>

GameMap::GameMap(int w, int h, int cs, int diff, TextureManager* texManager) :
    width(w), height(h), cellSize(cs), difficulty(diff), textureManager(texManager) {
    grid.resize(height, std::vector<char>(width, '.'));
    GenerateMap();
}

void GameMap::GenerateMap() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = '.';
        }
    }
    grid[0][0] = '.';

    int enemyCount = 5 + difficulty * 2;
    int placedEnemies = 0;

    while (placedEnemies < enemyCount) {
        int x = GetRandomValue(0, width - 1);
        int y = GetRandomValue(0, height - 1);

        if ((x == 0 && y == 0) || grid[y][x] != '.') continue;

        grid[y][x] = 'E';
        placedEnemies++;

        if (!IsPathExists({ 0, 0 }, { x, y })) {
            grid[y][x] = '.';
            placedEnemies--;
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ((x == 0 && y == 0) || grid[y][x] != '.') continue;
            int rnd = GetRandomValue(0, 9);
            if (rnd == 0) grid[y][x] = 'W';
            else if (rnd == 1) grid[y][x] = 'P';
        }
    }
}

bool GameMap::IsPathExists(Vector2i start, Vector2i end) const {
    if (start == end) return true;

    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<Vector2i> queue;

    queue.push(start);
    visited[start.y][start.x] = true;

    const int dx[4] = { -1, 1, 0, 0 };
    const int dy[4] = { 0, 0, -1, 1 };

    while (!queue.empty()) {
        Vector2i current = queue.front();
        queue.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                !visited[ny][nx] && grid[ny][nx] != 'W') {

                if (Vector2i{ nx, ny } == end)
                    return true;

                visited[ny][nx] = true;
                queue.push({ nx, ny });
            }
        }
    }

    return false;
}

void GameMap::Draw(int offsetX, int offsetY) const {
    DrawTexturePro(textureManager->GetMapBackground(),
        { 0, 0, (float)textureManager->GetMapBackground().width, (float)textureManager->GetMapBackground().height },
        { (float)offsetX, (float)offsetY, (float)(width * cellSize), (float)(height * cellSize) },
        { 0, 0 }, 0, WHITE);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Rectangle rect = { (float)(x * cellSize + offsetX),
                            (float)(y * cellSize + offsetY),
                            (float)cellSize, (float)cellSize };
            switch (grid[y][x]) {
            case 'W':
                DrawTexturePro(textureManager->GetWallTexture(),
                    { 0, 0, (float)textureManager->GetWallTexture().width, (float)textureManager->GetWallTexture().height },
                    rect, { 0, 0 }, 0, WHITE);
                break;
            case 'E':
                DrawTexturePro(textureManager->GetEnemyTexture(),
                    { 0, 0, (float)textureManager->GetEnemyTexture().width, (float)textureManager->GetEnemyTexture().height },
                    rect, { 0, 0 }, 0, WHITE);
                break;
            case 'P':
                DrawTexturePro(textureManager->GetPotionTexture(),
                    { 0, 0, (float)textureManager->GetPotionTexture().width, (float)textureManager->GetPotionTexture().height },
                    rect, { 0, 0 }, 0, WHITE);
                break;
            default:
                DrawRectangleLinesEx(rect, 1, DARKGRAY);
            }
        }
    }
}

char GameMap::GetCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return 'W';
    return grid[y][x];
}

void GameMap::ClearCell(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = '.';
    }
}