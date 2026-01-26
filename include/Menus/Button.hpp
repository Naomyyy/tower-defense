#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::Text text;
    sf::RectangleShape box;

    sf::Color normalColor   = sf::Color::White;      // Texto Branco
    sf::Color hoverColor    = sf::Color::Green;      // Hover Verde
    sf::Color pressedColor  = sf::Color(0, 100, 0);  // Clique Verde Escuro

    bool isHovered = false;
    bool isPressed = false;

public:
    Button(const std::string& label, sf::Vector2f pos, sf::Font& font, unsigned int size = 32);

    void update(sf::RenderWindow& win);
    bool clicked(sf::RenderWindow& win, const sf::Event& ev);

    void draw(sf::RenderWindow& win);

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const; // Útil para centralizar
};