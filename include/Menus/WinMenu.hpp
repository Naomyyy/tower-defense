#ifndef WINMENU_HPP
#define WINMENU_HPP

#include "MenuScreen.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class WinMenu : public MenuScreen {
public:
    explicit WinMenu(sf::RenderWindow& window);
    
    // Métodos obrigatórios herdados de MenuScreen
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;
    sf::Text mText;
    
    // Botão para voltar ao início
    std::unique_ptr<Button> mMenuButton;
    
    // Controla para qual estado o jogo deve ir após o clique
    MenuState mNextState = MenuState::None;

    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;

};

#endif