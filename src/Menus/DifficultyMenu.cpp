#include "Menus/DifficultyMenu.hpp"
#include <iostream>

DifficultyMenu::DifficultyMenu(sf::RenderWindow& window) {
    
    // Attempt to load the background image for the difficulty selection screen
    if (!backgroundTexture.loadFromFile("assets/menu-dif.jpg")) {
        std::cerr << "ERROR: Could not load Difficulty menu background!" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);

    // Calculate scaling factors to ensure the background fits the current window size
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);
    
    // Load the primary font for UI elements
    font.loadFromFile("assets/font.ttf");

    // Configure the title text properties and alignment
    title.setFont(font);
    title.setString("SELECT DIFFICULTY");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    
    // Center the title's origin based on its local bounds for perfect horizontal positioning
    sf::FloatRect textRect = title.getLocalBounds();
    title.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    title.setPosition(window.getSize().x / 2.f, 100.f);
    
    // Define button layout constants
    float spacing = 150.f; 
    float startX = window.getSize().x / 2.f - spacing;
    float posY = 250.f; 

    // Initialize difficulty buttons using smart pointers
    easyBtn   = std::make_unique<Button>("EASY",   sf::Vector2f(startX, posY), font, 40);
    normalBtn = std::make_unique<Button>("NORMAL", sf::Vector2f(startX + spacing, posY), font, 40);
    hardBtn   = std::make_unique<Button>("HARD",   sf::Vector2f(startX + 2 * spacing, posY), font, 40);
}

void DifficultyMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // Check for button interactions and update the state to launch gameplay
    if (easyBtn->clicked(window, ev))   { selection = 0; nextState = MenuState::Gameplay; }
    if (normalBtn->clicked(window, ev)) { selection = 1; nextState = MenuState::Gameplay; }
    if (hardBtn->clicked(window, ev))   { selection = 2; nextState = MenuState::Gameplay; }
}

void DifficultyMenu::update(float dt, sf::RenderWindow& window) {
    // Update button hover/active states
    easyBtn->update(window);
    normalBtn->update(window);
    hardBtn->update(window);
}

void DifficultyMenu::draw(sf::RenderWindow& window) {
    // Render all menu components to the current window
    window.draw(backgroundSprite);
    window.draw(title);
    
    easyBtn->draw(window);
    normalBtn->draw(window);
    hardBtn->draw(window);
}

int DifficultyMenu::getSelectedDifficulty() const { return selection; }

MenuState DifficultyMenu::getNextState() const { return nextState; }