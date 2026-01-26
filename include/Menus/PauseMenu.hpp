#pragma once
#include "MenuScreen.hpp"
#include "Button.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

class PauseMenu : public MenuScreen {
public:
    explicit PauseMenu(sf::RenderWindow& window); // Construtor

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;
    
    std::unique_ptr<Button> mResumeButton;
    std::unique_ptr<Button> mMainMenuButton;
    
    MenuState mNextState = MenuState::None;

    // --- ADICIONE ISSO PARA A TRANSPARÊNCIA ---
    sf::Texture mBackgroundTexture; // Guarda a imagem do jogo congelado
    sf::Sprite mBackgroundSprite;   // Mostra a imagem
    sf::RectangleShape mOverlay;    // O filtro escuro semitransparente
};