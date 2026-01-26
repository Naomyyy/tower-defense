#pragma once
#include "MenuScreen.hpp"
#include "Button.hpp"
#include <memory>
#include <SFML/Graphics.hpp> // Garanta que isso está incluido

class GameOverMenu : public MenuScreen {
public:
    explicit GameOverMenu(sf::RenderWindow& window);
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;
    sf::Text mText;
    std::unique_ptr<Button> mMenuButton;
    MenuState mNextState = MenuState::None;

    // --- ADICIONE ISTO AQUI EMBAIXO ---
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;
};