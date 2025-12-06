#pragma once
#include "MenuScreen.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

class MapEditor : public MenuScreen {
public:
    MapEditor(sf::RenderWindow& window, int tileSize);

    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    int mTileSize;

    int mGridWidth;
    int mGridHeight;

    std::vector<std::vector<bool>> mWalls; // mapa: parede = true, vazio = false

    bool mMouseDown = false;
    enum class PlacementMode { Wall, Erase };
    PlacementMode mCurrentMode = PlacementMode::Wall;

    MenuState mNextState = MenuState::None;

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};
