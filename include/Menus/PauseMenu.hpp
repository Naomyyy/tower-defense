#pragma once
#include "MenuScreen.hpp"
#include <SFML/Graphics.hpp>

class PauseMenu : public MenuScreen {
private:
    MenuState next = MenuState::None;
    sf::Font font;
    sf::Text resumeButton;

public:
    PauseMenu();

    void handleEvent(const sf::Event& ev) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
    MenuState getNextState() const override;
};
