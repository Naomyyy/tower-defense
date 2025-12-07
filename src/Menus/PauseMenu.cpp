#include "Menus/PauseMenu.hpp"

PauseMenu::PauseMenu() {
    font.loadFromFile("assets/font.ttf"); // Ajuste o caminho para sua fonte

    resumeButton.setFont(font);
    resumeButton.setString("Resume");
    resumeButton.setCharacterSize(36);
    resumeButton.setFillColor(sf::Color::White);
    resumeButton.setPosition(250.f, 250.f);
}

void PauseMenu::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);

        if (resumeButton.getGlobalBounds().contains(mousePos)) {
            next = MenuState::Gameplay;
        }
    }
}

void PauseMenu::update(float dt) {
    sf::Vector2i mousePosI = sf::Mouse::getPosition();
    sf::Vector2f mousePosF(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

    if (resumeButton.getGlobalBounds().contains(mousePosF)) {
        resumeButton.setFillColor(sf::Color::Yellow);
    } else {
        resumeButton.setFillColor(sf::Color::White);
    }
}

void PauseMenu::draw(sf::RenderWindow& win) {
    win.draw(resumeButton);
}

MenuState PauseMenu::getNextState() const {
    return next;
}
