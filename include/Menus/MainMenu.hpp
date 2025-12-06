#pragma once

#include <SFML/Graphics.hpp>
#include "MenuScreen.hpp"
class MainMenu : public MenuScreen {
private:
    MenuState next = MenuState::None;

    sf::Font font;
    sf::Text title;
    sf::Text playButton;
    sf::Text editorButton;

public:
    MainMenu();

    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
    MenuState getNextState() const override;
};
