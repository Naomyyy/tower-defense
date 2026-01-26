#include "Maps/Map.hpp"
#include <iostream>
#include <fstream>

Map::Map() {
    // Configura visual básico (depois usamos texturas)
     mGrassShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    mGrassShape.setFillColor(sf::Color(34, 139, 34)); // Verde Floresta

    mRoadShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    mRoadShape.setFillColor(sf::Color(194, 178, 128)); // Cor de areia/estrada

    mBuildableShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    mBuildableShape.setFillColor(sf::Color(0, 100, 255, 180)); // azul semi-transparente

    load("assets/map_normal.txt");

}

void Map::load(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open map file: " << filename << std::endl;
        return;
    }

    for(int y = 0; y < GRID_HEIGHT; ++y) {
        for(int x = 0; x < GRID_WIDTH; ++x) {
            if (!(file >> mGrid[y][x])) {
                mGrid[y][x] = 0; 
            }
        }
    }
    file.close();

    // Reinicia o sistema de waypoints para o caminho dos inimigos
    mPath.clear();
    int tempGrid[GRID_HEIGHT][GRID_WIDTH];
    for(int y=0; y<GRID_HEIGHT; ++y){
        for(int x=0; x<GRID_WIDTH; ++x){
            tempGrid[y][x] = mGrid[y][x];
        }
    }

    int currentX = -1;
    int currentY = -1;
        
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        if (tempGrid[y][0] == 1) {
            currentX = 0;
            currentY = y;
            break;
        }
    }

    if (currentX == -1) return; 

    mPath.push_back(getCenterCoords(currentX, currentY));
    tempGrid[currentY][currentX] = 9; // marca visitado na cópia

    int lastDirX = 1; 
    int lastDirY = 0; 
    int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    while (true) {
        bool foundNext = false;
        int nextX = -1, nextY = -1;
        int newDirX = 0, newDirY = 0;

        for (int i = 0; i < 4; ++i) {
            int checkX = currentX + dir[i][1];
            int checkY = currentY + dir[i][0];

            if (checkX >= 0 && checkX < GRID_WIDTH &&
                checkY >= 0 && checkY < GRID_HEIGHT &&
                tempGrid[checkY][checkX] == 1)
            {
                nextX = checkX;
                nextY = checkY;
                newDirX = dir[i][1];
                newDirY = dir[i][0];
                foundNext = true;
                break; 
            }
        }

        if (!foundNext) {
            mPath.push_back(getCenterCoords(currentX, currentY));
            break; 
        }

        if (newDirX != lastDirX || newDirY != lastDirY) {
            mPath.push_back(getCenterCoords(currentX, currentY));
            lastDirX = newDirX;
            lastDirY = newDirY;
        }

        currentX = nextX;
        currentY = nextY;
        tempGrid[currentY][currentX] = 9; // marca visitado
    }
}

void Map::draw(sf::RenderWindow& window) {
    for(int y=0; y<GRID_HEIGHT; ++y) {
        for(int x=0; x<GRID_WIDTH; ++x) {
            sf::Vector2f pos(x * TILE_SIZE, y * TILE_SIZE);

            if (mGrid[y][x] == 1) { // estrada
                mRoadShape.setPosition(pos);
                window.draw(mRoadShape);
            } 
            else if (mGrid[y][x] == 2) { // tile construível
                mBuildableShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                mBuildableShape.setPosition(pos);
                mBuildableShape.setFillColor(sf::Color(0, 100, 255, 180)); // azul semi-transparente
                window.draw(mBuildableShape);
            }
            else { // grama
                mGrassShape.setPosition(pos);
                window.draw(mGrassShape);
            }
        }
    }
}


int Map::getTileId(float mouseX, float mouseY) {
    int col = (int)(mouseX / TILE_SIZE);
    int row = (int)(mouseY / TILE_SIZE);

    if (col < 0 || col >= GRID_WIDTH || row < 0 || row >= GRID_HEIGHT) 
        return -1;

    return mGrid[row][col];
}

bool Map::isBuildable(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= GRID_WIDTH || tileY < 0 || tileY >= GRID_HEIGHT)
        return false;

    // Agora só tiles marcados com 2 são construíveis
    return mGrid[tileY][tileX] == 2;
}

const std::vector<sf::Vector2f>& Map::getPath() const { return mPath; }

sf::Vector2f Map::getCenterCoords(int x, int y) const {
    const float TILE_CENTER = TILE_SIZE / 2.f;
    return sf::Vector2f(x * TILE_SIZE + TILE_CENTER, y * TILE_SIZE + TILE_CENTER);
}
