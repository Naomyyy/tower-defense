#include "Menus/PauseMenu.hpp"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow& window)
    : mWindow(window)
{
    // -----------------------------------------------------------
    // 1. CAPTURAR A TELA DO JOGO (O TRUQUE DA TRANSPARÊNCIA)
    // -----------------------------------------------------------
    // Cria uma textura do tamanho da janela
    mBackgroundTexture.create(window.getSize().x, window.getSize().y);
    
    // Copia o conteúdo atual da janela (o jogo pausado) para a textura
    mBackgroundTexture.update(window);
    
    // Aplica a textura no sprite
    mBackgroundSprite.setTexture(mBackgroundTexture);

    // -----------------------------------------------------------
    // 2. CRIAR O FILTRO ESCURO (OVERLAY)
    // -----------------------------------------------------------
    mOverlay.setSize(sf::Vector2f(window.getSize()));
    
    // Cor Preta com Alpha 150 (0 = invisível, 255 = sólido)
    // Ajuste o 150 para mais claro ou mais escuro
    mOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // -----------------------------------------------------------
    // 3. SEU CÓDIGO DE BOTÕES E FONTE
    // -----------------------------------------------------------
    if (!mFont.loadFromFile("assets/font.ttf")) {
        // Trate erro
        std::cerr << "Erro ao carregar fonte no PauseMenu" << std::endl;
    }

    // Centralizando os botões
    float centerX = window.getSize().x / 2.0f;
    
    // Ajustei o X para centralizar com base num tamanho estimado ou fixo
    // (Se quiser centralização perfeita, use a lógica do BoundingBox que ensinei antes)
    mResumeButton = std::make_unique<Button>("Resume", sf::Vector2f(centerX - 100.f, 200.f), mFont, 32);
    mMainMenuButton = std::make_unique<Button>("Main Menu", sf::Vector2f(centerX - 120.f, 300.f), mFont, 32);
}

void PauseMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (mResumeButton->clicked(window, ev)) {
        mNextState = MenuState::Gameplay;
    } else if (mMainMenuButton->clicked(window, ev)) {
        mNextState = MenuState::MainMenu;
    }
}

void PauseMenu::update(float dt, sf::RenderWindow& window) {
    mResumeButton->update(window);
    mMainMenuButton->update(window);
}

void PauseMenu::draw(sf::RenderWindow& window) {
    // ATENÇÃO: NÃO use window.clear() aqui!
    // Se usar clear, você apaga tudo.
    
    // 1. Desenha o "print" do jogo congelado
    window.draw(mBackgroundSprite);

    // 2. Desenha o retângulo escuro por cima para dar destaque ao menu
    window.draw(mOverlay);

    // 3. Desenha os botões
    mResumeButton->draw(window);
    mMainMenuButton->draw(window);

}

MenuState PauseMenu::getNextState() const {
    return mNextState;
}