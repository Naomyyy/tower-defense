#pragma once
#include "MenuScreen.hpp"
#include "Menus/Button.hpp" 
#include <SFML/Graphics.hpp>

class MainMenu : public MenuScreen {
public:
    explicit MainMenu(sf::RenderWindow& window);
    ~MainMenu(); 

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& win) override;

    MenuState getNextState() const override;

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;

    Button* playButton;
   
    MenuState nextState = MenuState::None;

    sf::Texture backgroundTexture; 
    sf::Sprite backgroundSprite;
};