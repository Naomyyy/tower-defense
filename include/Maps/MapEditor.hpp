#pragma once
#include "Menus/MenuScreen.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MapEditor : public MenuScreen {
public:
    MapEditor(sf::RenderWindow& window, int tileSize);

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    MenuState getNextState() const override { return MenuState::None; }

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    sf::RenderWindow& mWindow;
    int mTileSize;

    int gridW, gridH;

    std::vector<std::vector<int>> tiles;

    bool mouseDown = false;
    bool placingWall = true;
};
