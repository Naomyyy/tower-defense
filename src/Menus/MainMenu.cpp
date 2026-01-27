#include "Menus/MainMenu.hpp"
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& window) : window(window) {
   
    // Attempt to load the main menu background texture
    if (!backgroundTexture.loadFromFile("assets/menu-image.png")) {
        std::cerr << "ERROR: Failed to load menu background texture!" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);

    // Calculate scaling factors to stretch the background image to fit the window
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);
    
    // Load the font before instantiating buttons
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "ERROR: Failed to load UI font!" << std::endl;
    }

    // Determine the center coordinates of the screen for UI alignment
    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    // Initialize the start button
    playButton = new Button("START", sf::Vector2f(centerX, centerY - 50), font, 60);
}

// Destructor: Responsible for manual memory management if using raw pointers
MainMenu::~MainMenu() {delete playButton;}

void MainMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    // Transition to the Difficulty Selection state if the play button is triggered
    if (playButton->clicked(window, ev)) {
        nextState = MenuState::Difficulty;
    }
}

void MainMenu::update(float dt, sf::RenderWindow& window) {
    // Handle button internal logic such as hover effects and color changes
    playButton->update(window);
}

void MainMenu::draw(sf::RenderWindow& win) {
    // Rendering order: background first, followed by UI components
    win.draw(backgroundSprite);
    playButton->draw(win);
}

MenuState MainMenu::getNextState() const {return nextState;}