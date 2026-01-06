#include "Menus/PauseMenu.hpp"

PauseMenu::PauseMenu(sf::RenderWindow& window)
    : mWindow(window)
{
    if (!mFont.loadFromFile("assets/font.ttf")) {
        // Trate erro de carregamento da fonte aqui
    }

    mResumeButton = std::make_unique<Button>("Resume", sf::Vector2f(300.f, 200.f), mFont, 32);
    mMainMenuButton = std::make_unique<Button>("Main Menu", sf::Vector2f(300.f, 300.f), mFont, 32);
}

void PauseMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (mResumeButton->clicked(window, ev)) {
        mNextState = MenuState::Gameplay;  // Voltar ao jogo
    } else if (mMainMenuButton->clicked(window, ev)) {
        mNextState = MenuState::MainMenu;  // Voltar ao menu principal
    }
}

void PauseMenu::update(float dt, sf::RenderWindow& window) {
    mResumeButton->update(window);
    mMainMenuButton->update(window);
}

void PauseMenu::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(50, 50, 50, 200)); // Fundo semitransparente
    mResumeButton->draw(window);
    mMainMenuButton->draw(window);
    window.display();
}

MenuState PauseMenu::getNextState() const {
    return mNextState;
}
