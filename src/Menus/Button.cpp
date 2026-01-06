#include "Menus/Button.hpp"

Button::Button(const std::string& label, sf::Vector2f pos, sf::Font& font, unsigned int size) {

    // Setup do texto
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);

    // Caixa do botão baseada no tamanho do texto
    box.setSize({
        text.getLocalBounds().width + 30.f,
        text.getLocalBounds().height + 20.f
    });

    box.setFillColor(normalColor);
    box.setPosition(pos);

    // Centralização do texto na caixa
    text.setPosition(
        pos.x + 15.f,
        pos.y + 10.f - text.getLocalBounds().top
    );
}

void Button::update(sf::RenderWindow& win) {
    sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(win);

    isHovered = box.getGlobalBounds().contains(mp);

    if (isHovered) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            box.setFillColor(pressedColor);
            isPressed = true;
        } else {
            box.setFillColor(hoverColor);
        }
    } else {
        box.setFillColor(normalColor);
        isPressed = false;
    }
}

bool Button::clicked(sf::RenderWindow& win, const sf::Event& ev) {
    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mp(ev.mouseButton.x, ev.mouseButton.y);

        return box.getGlobalBounds().contains(mp);
    }
    return false;
}

void Button::draw(sf::RenderWindow& win) {
    win.draw(box);
    win.draw(text);
}
