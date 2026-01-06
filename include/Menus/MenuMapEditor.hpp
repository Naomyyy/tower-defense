#pragma once
#include "MenuScreen.hpp"
#include "Button.hpp"
#include "../Maps/MapEditor.hpp"
#include <SFML/Graphics.hpp>

class MenuMapEditor : public MenuScreen {
public:
    MenuMapEditor(sf::RenderWindow& window);

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;

    Button* btnEdit;
    Button* btnLoad;
    Button* btnSave;
    Button* btnBack;

    MenuState next = MenuState::None;

    // Editor real
    MapEditor editor;
    bool editing = false;

    void enterEditor();
    void exitEditor();
};
