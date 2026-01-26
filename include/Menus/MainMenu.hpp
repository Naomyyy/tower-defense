#pragma once
#include "MenuScreen.hpp"
#include "Menus/Button.hpp" // <--- Inclua a sua classe Button
#include <SFML/Graphics.hpp>

class MainMenu : public MenuScreen {
public:
    explicit MainMenu(sf::RenderWindow& window);
    ~MainMenu(); // <--- Necessário para deletar os botões (free memory)

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& win) override;

    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font font;
    sf::Text title;

    Button* playButton;

   
    MenuState next = MenuState::None;

    sf::Texture backgroundTexture; 
    sf::Sprite backgroundSprite;
};