#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::Text text;
    sf::RectangleShape box;

    sf::Color normalColor   = sf::Color(200, 200, 200);
    sf::Color hoverColor    = sf::Color(255, 255, 150);
    sf::Color pressedColor  = sf::Color(180, 180, 100);

    bool isHovered = false;
    bool isPressed = false;

public:
    Button(const std::string& label, sf::Vector2f pos, sf::Font& font, unsigned int size = 32);

    void update(sf::RenderWindow& win);
    bool clicked(sf::RenderWindow& win, const sf::Event& ev);

    void draw(sf::RenderWindow& win);

    sf::Vector2f getPosition() const;
};
#pragma once