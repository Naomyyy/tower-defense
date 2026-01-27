#include "Maps/Map.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

Map::Map() {
    // Setup basic tile visuals
    grassShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    grassShape.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    roadShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    roadShape.setFillColor(sf::Color(194, 178, 128)); // Sand/Road Color

    buildableShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    buildableShape.setFillColor(sf::Color(0, 100, 255, 180)); // Blue 

    load("assets/map_normal.txt");

}

void Map::load(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open map file: " << filename << std::endl;
        return;
    }
    // Read the grid data from the file
    for(int y = 0; y < GRID_HEIGHT; ++y) {
        for(int x = 0; x < GRID_WIDTH; ++x) {
            if (!(file >> grid[y][x])) {
                grid[y][x] = 0; // Default to grass if reading fails
            }
        }
    }
    file.close();

    // Reset waypoint system for enemy movement
    path.clear();
    int tempGrid[GRID_HEIGHT][GRID_WIDTH];
    for(int y=0; y<GRID_HEIGHT; ++y){
        for(int x=0; x<GRID_WIDTH; ++x){
            tempGrid[y][x] = grid[y][x];
        }
    }
    int currentX = -1;
    int currentY = -1;
    // Find the starting point (assumed to be on the left edge, col 0)
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        if (tempGrid[y][0] == 1) {
            currentX = 0;
            currentY = y;
            break;
        }
    }

    if (currentX == -1) return; 
    // Add the first waypoint and mark as visited
    path.push_back(getCenterCoords(currentX, currentY));
    tempGrid[currentY][currentX] = 9; 

    int lastDirX = 1; 
    int lastDirY = 0; 
    int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    
    
    // PATHFINDING  : Follows the road tiles and creates waypoints only when the path changes direction
     
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
        // Path ended: Add the last position as the final waypoint
        if (!foundNext) {
            path.push_back(getCenterCoords(currentX, currentY));
            break; 
        }
        // If the direction changed, we mark the previous tile as a waypoint
        if (newDirX != lastDirX || newDirY != lastDirY) {
            path.push_back(getCenterCoords(currentX, currentY));
            lastDirX = newDirX;
            lastDirY = newDirY;
        }

        currentX = nextX;
        currentY = nextY;
        tempGrid[currentY][currentX] = 9; //  Mark as visited to avoid backtracking
    }
}

void Map::draw(sf::RenderWindow& window) {
    for(int y=0; y<GRID_HEIGHT; ++y) {
        for(int x=0; x<GRID_WIDTH; ++x) {
            sf::Vector2f pos(x * TILE_SIZE, y * TILE_SIZE);

            if (grid[y][x] == 1) { // Road
                roadShape.setPosition(pos);
                window.draw(roadShape);
            } 
            else if (grid[y][x] == 2) { // Buildable
                buildableShape.setPosition(pos);
                window.draw(buildableShape);
            }
            else { // grass
                grassShape.setPosition(pos);
                window.draw(grassShape);
            }
        }
    }
}


int Map::getTileId(float mouseX, float mouseY) {
    int col = (int)std::floor(mouseX / TILE_SIZE);
    int row = (int)std::floor(mouseY / TILE_SIZE);

    if (col < 0 || col >= GRID_WIDTH || row < 0 || row >= GRID_HEIGHT) 
        return -1;

    return grid[row][col];
}

bool Map::isBuildable(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= GRID_WIDTH || tileY < 0 || tileY >= GRID_HEIGHT)
        return false;
    return grid[tileY][tileX] == 2;
}

const std::vector<sf::Vector2f>& Map::getPath() const { 
    return path; 
}

sf::Vector2f Map::getCenterCoords(int x, int y) const {
    const float TILE_CENTER = TILE_SIZE / 2.f;
    return sf::Vector2f(x * TILE_SIZE + TILE_CENTER, y * TILE_SIZE + TILE_CENTER);
}
