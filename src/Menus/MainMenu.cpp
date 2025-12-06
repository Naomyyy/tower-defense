#include "Menus/MainMenu.hpp"

MainMenu::MainMenu() {
    font.loadFromFile("assets/font.ttf"); // Ajuste o caminho para sua fonte

    title.setFont(font);
    title.setString("Tower Defense");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setPosition(200.f, 50.f);

    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(36);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(250.f, 200.f);

    editorButton.setFont(font);
    editorButton.setString("Map Editor");
    editorButton.setCharacterSize(36);
    editorButton.setFillColor(sf::Color::White);
    editorButton.setPosition(250.f, 280.f);
}

void MainMenu::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);

        if (playButton.getGlobalBounds().contains(mousePos)) {
            next = MenuState::Gameplay;
        }
        else if (editorButton.getGlobalBounds().contains(mousePos)) {
            next = MenuState::MapEditor;
        }
    }
}

void MainMenu::update(float dt) {
    // Você pode colocar efeito hover nos botões aqui se quiser
    // Exemplo básico para mudar cor se mouse estiver sobre botão:

    sf::Vector2i mousePosI = sf::Mouse::getPosition();
    sf::Vector2f mousePosF(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

    if (playButton.getGlobalBounds().contains(mousePosF)) {
        playButton.setFillColor(sf::Color::Yellow);
    } else {
        playButton.setFillColor(sf::Color::White);
    }

    if (editorButton.getGlobalBounds().contains(mousePosF)) {
        editorButton.setFillColor(sf::Color::Yellow);
    } else {
        editorButton.setFillColor(sf::Color::White);
    }
}

void MainMenu::draw(sf::RenderWindow& win) {
    win.draw(title);
    win.draw(playButton);
    win.draw(editorButton);
}

MenuState MainMenu::getNextState() const {
    return next;
}
