#pragma once
#include "MenuScreen.hpp"
#include <SFML/Graphics.hpp>

class MainMenu : public MenuScreen {
public:
    explicit MainMenu(sf::RenderWindow& window);

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& win) override;

    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font font;
    sf::Text title;
    sf::Text playButton;
    sf::Text editorButton;
    MenuState next = MenuState::None;
};
#pragma once