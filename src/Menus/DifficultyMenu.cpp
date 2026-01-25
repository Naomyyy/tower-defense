#include "Menus/DifficultyMenu.hpp"

DifficultyMenu::DifficultyMenu(sf::RenderWindow& window) {
    mFont.loadFromFile("assets/font.ttf");

    mTitle.setFont(mFont);
    mTitle.setString("SELECT DIFFICULTY");
    mTitle.setCharacterSize(50);
    mTitle.setFillColor(sf::Color::White);
    mTitle.setPosition(window.getSize().x / 2.f - 200.f, 100.f);

    float centerX = window.getSize().x / 2.f - 60.f;
    mEasyBtn = std::make_unique<Button>("EASY", sf::Vector2f(centerX, 250.f), mFont, 30);
    mNormalBtn = std::make_unique<Button>("NORMAL", sf::Vector2f(centerX, 330.f), mFont, 30);
    mHardBtn = std::make_unique<Button>("HARD", sf::Vector2f(centerX, 410.f), mFont, 30);
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
    window.clear(sf::Color(30, 30, 30));
    window.draw(mTitle);
    mEasyBtn->draw(window);
    mNormalBtn->draw(window);
    mHardBtn->draw(window);
}

int DifficultyMenu::getSelectedDifficulty() const {return mSelection; };


MenuState DifficultyMenu::getNextState() const { return mNextState; }

