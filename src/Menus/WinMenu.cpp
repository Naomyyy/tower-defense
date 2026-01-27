#include "Menus/WinMenu.hpp"
#include <iostream>

WinMenu::WinMenu(sf::RenderWindow& window) : window(window) {

    // Load the victory background asset from the assets folder
    if (!backgroundTexture.loadFromFile("assets/victory-image.png")) {
        std::cerr << "ERROR: Failed to load assets/victory-image.png" << std::endl;
        // If the load fails, the sprite will remain white, but execution continues.
    }

    backgroundSprite.setTexture(backgroundTexture);

    // Scale the background sprite to cover the entire window area
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    
    backgroundSprite.setScale(scaleX, scaleY);

    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "ERROR: Failed to load font in WinMenu" << std::endl;
    }
   
    std::string btnText = "RETURN TO MENU";
    int btnSize = 60;

    // Measure the local bounds of the button text for precise centering
    sf::Text tempText(btnText, font, btnSize);
    sf::FloatRect btnBounds = tempText.getLocalBounds();

    // Instantiate the menu button using a smart pointer for automated resource management
    menuButton = std::make_unique<Button>(btnText, sf::Vector2f(windowSize.x / 2.0f, 300.f), font, btnSize);
}

void WinMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // Check for interaction and trigger transition back to the Main Menu
    if (menuButton->clicked(window, ev)) {
        nextState = MenuState::MainMenu;
    }
}

void WinMenu::update(float dt, sf::RenderWindow& window) {
    menuButton->update(window);
}

void WinMenu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(text);
    menuButton->draw(window);
}

MenuState WinMenu::getNextState() const {
    return nextState;
}