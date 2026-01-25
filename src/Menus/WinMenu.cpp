#include "Menus/WinMenu.hpp"
#include <iostream>

WinMenu::WinMenu(sf::RenderWindow& window) : mWindow(window) {
    // Carrega a fonte (certifique-se de que font.ttf existe em assets)
    if (!mFont.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error loading font in WinMenu" << std::endl;
    }

    // Configuração do texto de Vitória
    mText.setFont(mFont);
    mText.setString("YOU WON!");
    mText.setCharacterSize(60);
    mText.setFillColor(sf::Color::White);
    
    // Centralização básica do texto
    sf::FloatRect textBounds = mText.getLocalBounds();
    mText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    mText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 4.0f);
    float buttonX = (window.getSize().x / 2.0f) - (75.0f); // 75.f é metade da largura média do botão
    float buttonY = window.getSize().y / 1.8f; // Posiciona um pouco abaixo do meio
    
    mMenuButton = std::make_unique<Button>("RETURN TO MENU", sf::Vector2f(buttonX, buttonY), mFont, 24);
}

void WinMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // Se o botão for clicado, define o próximo estado para MainMenu
    if (mMenuButton->clicked(window, ev)) {
        mNextState = MenuState::MainMenu;
    }
}

void WinMenu::update(float dt, sf::RenderWindow& window) {
    // Atualiza o estado visual do botão (hover, etc)
    mMenuButton->update(window);
}

void WinMenu::draw(sf::RenderWindow& window) {
    // Limpa com um fundo escuro para destacar o texto verde
    window.clear(sf::Color(0, 20, 0)); 
    
    window.draw(mText);
    mMenuButton->draw(window);
}

MenuState WinMenu::getNextState() const {
    return mNextState;
}