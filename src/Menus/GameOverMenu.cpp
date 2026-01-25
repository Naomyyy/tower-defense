#include "Menus/GameOverMenu.hpp"

GameOverMenu::GameOverMenu(sf::RenderWindow& window) : mWindow(window) {
    mFont.loadFromFile("assets/font.ttf");
    mText.setFont(mFont);
    mText.setString("GAME OVER");
    mText.setCharacterSize(60);
    mText.setFillColor(sf::Color::White);
    mText.setPosition(250.f, 150.f);

    mMenuButton = std::make_unique<Button>("Menu", sf::Vector2f(300.f, 350.f), mFont, 32);
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
    window.clear(sf::Color(20, 0, 0)); // Fundo avermelhado escuro
    window.draw(mText);
    mMenuButton->draw(window);
}

MenuState GameOverMenu::getNextState() const { return mNextState; }