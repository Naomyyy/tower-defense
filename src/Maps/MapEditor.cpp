#include "Maps/MapEditor.hpp"
#include <fstream>
#include <iostream>

MapEditor::MapEditor(sf::RenderWindow& window, int tile)
    : mWindow(window), mTileSize(tile)
{
    gridW = window.getSize().x / tile;
    gridH = window.getSize().y / tile;

    tiles.assign(gridH, std::vector<int>(gridW, 0));
}

void MapEditor::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (ev.type == sf::Event::MouseButtonPressed) {
        mouseDown = true;
        placingWall = (ev.mouseButton.button == sf::Mouse::Left);
    }
    if (ev.type == sf::Event::MouseButtonReleased) {
        mouseDown = false;
    }
}

void MapEditor::update(float dt, sf::RenderWindow& window) {
    if (!mouseDown) return;

    sf::Vector2i mp = sf::Mouse::getPosition(window);
    int gx = mp.x / mTileSize;
    int gy = mp.y / mTileSize;

    if (gx >= 0 && gx < gridW && gy >= 0 && gy < gridH)
        tiles[gy][gx] = placingWall ? 1 : 0;
}

void MapEditor::draw(sf::RenderWindow& window) {
    sf::RectangleShape rect({(float)mTileSize, (float)mTileSize});

    for (int y = 0; y < gridH; y++) {
        for (int x = 0; x < gridW; x++) {
            rect.setPosition(x * mTileSize, y * mTileSize);
            rect.setFillColor(tiles[y][x] ? sf::Color::Black : sf::Color(200, 200, 200));
            window.draw(rect);
        }
    }
}

void MapEditor::saveToFile(const std::string& filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return;

    for (auto& row : tiles) {
        for (int v : row) f << v;
        f << "\n";
    }
}

void MapEditor::loadFromFile(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return;

    std::string line;
    int y = 0;

    while (std::getline(f, line) && y < gridH) {
        for (int x = 0; x < (int)line.size() && x < gridW; x++)
            tiles[y][x] = (line[x] == '1') ? 1 : 0;
        y++;
    }
}
