#include "Menus/MainMenu.hpp"
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& window) : mWindow(window) {
    // --- INICIO DA CONFIGURAÇÃO DO FUNDO ---
    if (!backgroundTexture.loadFromFile("assets/menu-image.png")) {
        std::cerr << "ERRO: Nao foi possivel carregar o fundo do menu!" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = mWindow.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);
    // --- FIM DA CONFIGURAÇÃO DO FUNDO ---


    // --- CONFIGURAÇÃO DOS BOTÕES ---
    
    // 1. Carregar a fonte ANTES de criar os botões
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "ERRO: Nao foi possivel carregar a fonte!" << std::endl;
    }

    // Pegamos o centro da tela
    float centerX = mWindow.getSize().x / 2.0f;
    float centerY = mWindow.getSize().y / 2.0f;

    // Criamos os botões alinhados no centro X
    // Botão Play um pouco acima do centro Y
    playButton = new Button("START", sf::Vector2f(centerX, centerY - 50), font, 60);

}

// Destrutor: Importante para limpar a memória usada pelo 'new'
MainMenu::~MainMenu() {
    delete playButton;

}

void MainMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // A classe Button já verifica se foi clicada com o botão esquerdo
  
    if (playButton->clicked(window, ev)) {
        
        next = MenuState::Difficulty;
    }
    

}

void MainMenu::update(float dt, sf::RenderWindow& window) {
    // Chama o update do próprio botão (que cuida da cor e do hover)
    playButton->update(window);
  
}

void MainMenu::draw(sf::RenderWindow& win) {
    // 1. DESENHA O FUNDO PRIMEIRO
    win.draw(backgroundSprite);
    playButton->draw(win);

}

MenuState MainMenu::getNextState() const {
    return next;
}