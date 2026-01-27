#pragma once
#include "MenuScreen.hpp"
#include "Button.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

class PauseMenu : public MenuScreen {
public:
    explicit PauseMenu(sf::RenderWindow& window); 

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    MenuState getNextState() const override;

private:
    sf::RenderWindow& window;
    sf::Font font;
    
    std::unique_ptr<Button> resumeButton;
    std::unique_ptr<Button> mainMenuButton;
    
    MenuState nextState = MenuState::None;

    sf::Texture backgroundTexture; 
    sf::Sprite backgroundSprite;   
    sf::RectangleShape overlay;    
};