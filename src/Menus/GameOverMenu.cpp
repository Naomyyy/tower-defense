#include "Menus/GameOverMenu.hpp"
#include <iostream> 

GameOverMenu::GameOverMenu(sf::RenderWindow& window) : window(window) {
    // Attempt to load the Game Over asset from the assets folder
    if (!backgroundTexture.loadFromFile("assets/gamerover-image.png")) {
        std::cerr << "ERROR: Failed to load assets/gamerover-image.png" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    
    // Calculate scale factors to match the background sprite to the current window dimensions
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // --- 2. TITLE CONFIGURATION ---
    font.loadFromFile("assets/font.ttf");
    text.setFont(font);
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::White);

    // Center the text origin to ensure precise positioning relative to its center
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    
    // Place the title in the horizontal center of the screen at Y = 150
    text.setPosition(windowSize.x / 2.0f, 150.f);

    // --- 3. BUTTON INITIALIZATION ---
    std::string btnLabel = "Play again";
    int btnFontSize = 60;

    // Instantiate the menu button using a smart pointer for automatic memory management
    menuButton = std::make_unique<Button>(btnLabel, sf::Vector2f(windowSize.x / 2.0f, 300.f), font, btnFontSize);
}

void GameOverMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // Trigger transition back to the Main Menu if the button is clicked
    if (menuButton->clicked(window, ev)) {
        nextState = MenuState::MainMenu;
    }
}

void GameOverMenu::update(float dt, sf::RenderWindow& window) {
    // Update button visual states (e.g., hover effects)
    menuButton->update(window);
}

void GameOverMenu::draw(sf::RenderWindow& window) {
    // Render order: draw the background first, then UI elements on top
    window.draw(backgroundSprite); 
    window.draw(text);
    menuButton->draw(window);
}

MenuState GameOverMenu::getNextState() const { return nextState; }