#include "Menus/MenuMapEditor.hpp"
#include <iostream>

MenuMapEditor::MenuMapEditor(sf::RenderWindow& window)
    : mWindow(window), editor(window, 32)
{
    mFont.loadFromFile("assets/font.ttf");

    btnEdit = new Button("Editar Mapa", {100, 120}, mFont);
    btnLoad = new Button("Carregar",    {100, 200}, mFont);
    btnSave = new Button("Salvar",      {100, 280}, mFont);
    btnBack = new Button("Voltar",      {100, 360}, mFont);
}

void MenuMapEditor::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (!editing) {
        if (btnEdit->clicked(window, ev)) {
            enterEditor();
        }
        if (btnLoad->clicked(window, ev)) {
            editor.loadFromFile("map.txt");
        }
        if (btnSave->clicked(window, ev)) {
            editor.saveToFile("map.txt");
        }
        if (btnBack->clicked(window, ev)) {
            next = MenuState::MainMenu;
        }
    }
    else {
        editor.handleEvent(ev, window);
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
            exitEditor();
    }
}

void MenuMapEditor::update(float dt, sf::RenderWindow& window) {
    if (!editing) {
        btnEdit->update(window);
        btnLoad->update(window);
        btnSave->update(window);
        btnBack->update(window);
    }
    else {
        editor.update(dt, window);
    }
}

void MenuMapEditor::draw(sf::RenderWindow& window) {
    if (!editing) {
        btnEdit->draw(window);
        btnLoad->draw(window);
        btnSave->draw(window);
        btnBack->draw(window);
    }
    else {
        editor.draw(window);
    }
}

MenuState MenuMapEditor::getNextState() const {
    return next;
}

void MenuMapEditor::enterEditor() {
    editing = true;
}

void MenuMapEditor::exitEditor() {
    editing = false;
}
