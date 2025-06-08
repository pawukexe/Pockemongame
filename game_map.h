#pragma once

#include <vector>
#include <raylib.h>
#include "texture_manager.h"

struct Vector2i {
    int x, y;
    bool operator==(const Vector2i& other) const { return x == other.x && y == other.y; }
};

class GameMap {
private:
    std::vector<std::vector<char>> grid;
    int width, height;
    int cellSize;
    int difficulty;
    TextureManager* textureManager;

    bool IsPathExists(Vector2i start, Vector2i end) const;

public:
    GameMap(int w, int h, int cs, int diff, TextureManager* texManager);
    void GenerateMap();
    void Draw(int offsetX, int offsetY) const;
    char GetCell(int x, int y) const;
    void ClearCell(int x, int y);
    int GetCellSize() const { return cellSize; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetDifficulty() const { return difficulty; }
};

