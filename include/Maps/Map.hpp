#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Global constants for the grid dimensions
const int TILE_SIZE = 40;
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 15;

class Map {
public:
    Map();
    // Loads map data from a text file
    void load(const std::string& filename);
    // Renders the entire grid to the screen
    void draw(sf::RenderWindow& window);
    // Converts screen pixel coordinates to a tile ID from the grid
    int getTileId(float mouseX, float mouseY);
    // Checks if a specific tile allows tower construction
    bool isBuildable(int tileX, int tileY) const;
    // Returns the list of waypoints for enemy movement
    const std::vector<sf::Vector2f>& getPath() const;
    // Calculates the center pixel coordinates of a specific tile.
    sf::Vector2f getCenterCoords(int x, int y) const;

private:
    int grid[GRID_HEIGHT][GRID_WIDTH];
    std::vector<sf::Vector2f> path;
    // Visual shapes for different tile types
    sf::RectangleShape grassShape;     
    sf::RectangleShape roadShape;       
    sf::RectangleShape buildableShape;  
};
