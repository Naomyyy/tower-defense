#include "Menus/DifficultyMenu.hpp"
#include <iostream> // Necessário para avisar se a imagem der erro

DifficultyMenu::DifficultyMenu(sf::RenderWindow& window) {
    // --- 1. CONFIGURAÇÃO DA IMAGEM DE FUNDO ---
    // Mude "assets/difficulty_bg.png" para o nome do arquivo que você quer usar
    if (!mBackgroundTexture.loadFromFile("assets/menu-dif.jpg")) {
        std::cerr << "ERRO: Nao foi possivel carregar o fundo de Dificuldade!" << std::endl;
    }

    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Cálculos para esticar a imagem e cobrir a tela inteira
    sf::Vector2u textureSize = mBackgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;

    mBackgroundSprite.setScale(scaleX, scaleY);
    // ------------------------------------------

    // --- SEU CÓDIGO ORIGINAL ---
    mFont.loadFromFile("assets/font.ttf");

    mTitle.setFont(mFont);
    mTitle.setString("SELECT DIFFICULTY");
    mTitle.setCharacterSize(50);
    mTitle.setFillColor(sf::Color::White);
    
    // Dica: Se quiser centralizar o título perfeitamente igual fizemos no outro menu:
    sf::FloatRect textRect = mTitle.getLocalBounds();
    mTitle.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    mTitle.setPosition(window.getSize().x / 2.f, 100.f);
    
    // Se preferir manter o seu posicionamento manual antigo, descomente a linha abaixo e apague as 3 acima:
    // mTitle.setPosition(window.getSize().x / 2.f - 200.f, 100.f);


    // Espaçamento entre os botões
    float spacing = 150.f; // distância entre os botões
    float startX = window.getSize().x / 2.f - spacing; // centraliza o conjunto
    float posY = 250.f; // mesma altura para todos

    mEasyBtn   = std::make_unique<Button>("EASY",   sf::Vector2f(startX, posY), mFont, 30);
    mNormalBtn = std::make_unique<Button>("NORMAL", sf::Vector2f(startX + spacing, posY), mFont, 30);
    mHardBtn   = std::make_unique<Button>("HARD",   sf::Vector2f(startX + 2*spacing, posY), mFont, 30);

}

void DifficultyMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (mEasyBtn->clicked(window, ev)) { mSelection = 0; mNextState = MenuState::Gameplay; }
    if (mNormalBtn->clicked(window, ev)) { mSelection = 1; mNextState = MenuState::Gameplay; }
    if (mHardBtn->clicked(window, ev)) { mSelection = 2; mNextState = MenuState::Gameplay; }
}

void DifficultyMenu::update(float dt, sf::RenderWindow& window) {
    mEasyBtn->update(window);
    mNormalBtn->update(window);
    mHardBtn->update(window);
}

void DifficultyMenu::draw(sf::RenderWindow& window) {
    // 1. DESENHA O FUNDO PRIMEIRO
    // Substitui o window.clear
    window.draw(mBackgroundSprite);

    // 2. DESENHA O RESTO
    window.draw(mTitle);
    mEasyBtn->draw(window);
    mNormalBtn->draw(window);
    mHardBtn->draw(window);
}

int DifficultyMenu::getSelectedDifficulty() const { return mSelection; };

MenuState DifficultyMenu::getNextState() const { return mNextState; }