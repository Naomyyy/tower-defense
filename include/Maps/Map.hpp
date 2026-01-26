#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


// Tamanho de cada quadrado no mapa (40x40 pixels)
const int TILE_SIZE = 40;
// Quantidade de blocos na tela (800 / 40 = 20, 600 / 40 = 15)
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 15;

class Map {
public:
    Map();
    void load(const std::string& filename);
    void draw(sf::RenderWindow& window);
    int getTileId(float mouseX, float mouseY);
    bool isBuildable(int tileX, int tileY) const;
    const std::vector<sf::Vector2f>& getPath() const;
    sf::Vector2f getCenterCoords(int x, int y) const;

private:
    int mGrid[GRID_HEIGHT][GRID_WIDTH];
    std::vector<sf::Vector2f> mPath;

    sf::RectangleShape mGrassShape;      // já existia
    sf::RectangleShape mRoadShape;       // já existia

    // ✅ Adicione isto:
    sf::RectangleShape mBuildableShape;  // para tiles onde o jogador pode construir
};
