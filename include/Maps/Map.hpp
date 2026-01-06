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
    
    void load(); 
    void draw(sf::RenderWindow& window);

    // Helps to know if we clicked on grass or road
    int getTileId(float x, float y); 

private:
    // The matrix that stores the ID of each block [y][x]
    int mGrid[GRID_HEIGHT][GRID_WIDTH];

    // Simple shapes to draw (later you can replace with Sprites/Images)
    sf::RectangleShape mGrassShape;
    sf::RectangleShape mRoadShape;
};