#include "Menus/WinMenu.hpp"
#include <iostream>

WinMenu::WinMenu(sf::RenderWindow& window) : mWindow(window) {
    // -----------------------------------------------------------
    // 1. CONFIGURAÇÃO DA IMAGEM DE FUNDO
    // -----------------------------------------------------------
    // Certifique-se de ter uma imagem de vitória na pasta assets!
    if (!mBackgroundTexture.loadFromFile("assets/victory-image.png")) {
        std::cerr << "ERRO: Nao foi possivel carregar assets/victory-image.png" << std::endl;
        // Se falhar, o sprite ficará branco, mas o jogo não fecha.
    }

    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Ajusta a escala para preencher a janela (igual fizemos nos outros)
    sf::Vector2u textureSize = mBackgroundTexture.getSize();
    sf::Vector2u windowSize = mWindow.getSize();
    
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    
    mBackgroundSprite.setScale(scaleX, scaleY);

    // -----------------------------------------------------------
    // 2. CONFIGURAÇÃO DO TEXTO E BOTÃO
    // -----------------------------------------------------------
    if (!mFont.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error loading font in WinMenu" << std::endl;
    }
   
    // --- CÁLCULO PARA CENTRALIZAR O BOTÃO PERFEITAMENTE ---
    std::string btnText = "RETURN TO MENU";
    int btnSize = 60;

    // Mede o tamanho que o texto do botão terá
    sf::Text tempText(btnText, mFont, btnSize);
    sf::FloatRect btnBounds = tempText.getLocalBounds();

    mMenuButton = std::make_unique<Button>(btnText, sf::Vector2f(windowSize.x / 2.0f, 300.f), mFont, btnSize);
}

void WinMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (mMenuButton->clicked(window, ev)) {
        mNextState = MenuState::MainMenu;
    }
}

void WinMenu::update(float dt, sf::RenderWindow& window) {
    mMenuButton->update(window);
}

void WinMenu::draw(sf::RenderWindow& window) {
    // 1. DESENHA O FUNDO PRIMEIRO (Removemos o clear com cor sólida)
    window.draw(mBackgroundSprite);
    
    // 2. DESENHA O RESTO POR CIMA
    window.draw(mText);
    mMenuButton->draw(window);
}

MenuState WinMenu::getNextState() const {
    return mNextState;
}