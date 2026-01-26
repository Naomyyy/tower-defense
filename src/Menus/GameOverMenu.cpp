#include "Menus/GameOverMenu.hpp"
#include <iostream> // <--- Necessário para mostrar erro se a imagem falhar

GameOverMenu::GameOverMenu(sf::RenderWindow& window) : mWindow(window) {
    // --- 1. CONFIGURAÇÃO DA IMAGEM DE FUNDO ---
    if (!mBackgroundTexture.loadFromFile("assets/gamerover-image.png")) {
        std::cerr << "ERRO: Nao foi possivel carregar assets/gamerover-image.png" << std::endl;
    }
    mBackgroundSprite.setTexture(mBackgroundTexture);
    
    // Ajuste de escala do fundo
    sf::Vector2u textureSize = mBackgroundTexture.getSize();
    sf::Vector2u windowSize = mWindow.getSize();
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    mBackgroundSprite.setScale(scaleX, scaleY);

    // --- 2. CONFIGURAÇÃO DO TÍTULO ---
    mFont.loadFromFile("assets/font.ttf");
    mText.setFont(mFont);
    mText.setCharacterSize(60);
    mText.setFillColor(sf::Color::White);

    // Centraliza o ponto de origem do texto no meio dele mesmo
    sf::FloatRect textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    
    // Posiciona o título no meio da tela (e um pouco para cima, no Y = 150)
    mText.setPosition(windowSize.x / 2.0f, 150.f);

    // --- 3. CÁLCULO DA POSIÇÃO DO BOTÃO ---
    std::string btnLabel = "Play again";
    int btnFontSize = 60;

    mMenuButton = std::make_unique<Button>(btnLabel, sf::Vector2f(windowSize.x / 2.0f, 300.f), mFont, btnFontSize);
}


void GameOverMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (mMenuButton->clicked(window, ev)) {
        mNextState = MenuState::MainMenu;
    }
}

void GameOverMenu::update(float dt, sf::RenderWindow& window) {
    mMenuButton->update(window);
}

void GameOverMenu::draw(sf::RenderWindow& window) {
    // Nota: Removi o window.clear(Color) pois a imagem vai cobrir o fundo.
    
    // 1. DESENHA O FUNDO PRIMEIRO
    window.draw(mBackgroundSprite); 

    // 2. DESENHA O TEXTO E BOTÃO POR CIMA
    window.draw(mText);
    mMenuButton->draw(window);
}

MenuState GameOverMenu::getNextState() const { return mNextState; }