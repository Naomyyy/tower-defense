#include "Menus/PauseMenu.hpp"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow& window)
    : window(window)
{
    // Initialize the texture with the current window dimensions
    backgroundTexture.create(window.getSize().x, window.getSize().y);
    
    // Copy the current frame buffer (the paused game state) into the texture
    backgroundTexture.update(window);
    
    // Assign the captured texture to the background sprite
    backgroundSprite.setTexture(backgroundTexture);

    overlay.setSize(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "ERROR: Failed to load font in PauseMenu" << std::endl;
    }

    // Determine the horizontal center of the screen for UI alignment
    float centerX = window.getSize().x / 2.0f;
    
    // Instantiate navigation buttons using smart pointers for safe memory management
    resumeButton = std::make_unique<Button>("Resume", sf::Vector2f(centerX - 100.f, 200.f), font, 32);
    mainMenuButton = std::make_unique<Button>("Main Menu", sf::Vector2f(centerX - 120.f, 300.f), font, 32);
}

void PauseMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // Handle menu interactions and set the next state for the game engine
    if (resumeButton->clicked(window, ev)) {
        nextState = MenuState::Gameplay;
    } else if (mainMenuButton->clicked(window, ev)) {
        nextState = MenuState::MainMenu;
    }
}

void PauseMenu::update(float dt, sf::RenderWindow& window) {
    // Update button visual feedback 
    resumeButton->update(window);
    mainMenuButton->update(window);
}

void PauseMenu::draw(sf::RenderWindow& window) {
    
    //Draw the "snapshot" of the paused game
    window.draw(backgroundSprite);

    // Draw the dark overlay to prioritize the menu visibility
    window.draw(overlay);

    // Render the UI components on top
    resumeButton->draw(window);
    mainMenuButton->draw(window);
}

MenuState PauseMenu::getNextState() const {
    return nextState;
}