#include "Menus/Button.hpp"

Button::Button(const std::string& label, sf::Vector2f pos, sf::Font& font, unsigned int size) {
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(normalColor); 

    // Retrieve the local bounding box to calculate the exact center of the text
    sf::FloatRect textRect = text.getLocalBounds();
    
    // Set the origin to the center of the text rectangle to allow precise positioning
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top  + textRect.height / 2.0f);
    
    // Set the final position of the text on the screen
    text.setPosition(pos);

    // Add padding to make the clickable area slightly larger than the text for better UX
    sf::Vector2f padding(20.f, 10.f);
    box.setSize(sf::Vector2f(textRect.width + padding.x * 2, textRect.height + padding.y * 2));
    
    
    box.setOrigin(box.getSize() / 2.0f);
    box.setPosition(pos);
    box.setFillColor(sf::Color::Transparent); 
}

void Button::update(sf::RenderWindow& win) {
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(win);
    
    // Check if the mouse cursor is hovering over the button's global bounds
    isHovered = box.getGlobalBounds().contains(mousePos);

    if (isHovered) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            text.setFillColor(pressedColor); 
        } else {
            text.setFillColor(hoverColor);   
        }
    } else {
        text.setFillColor(normalColor); 
    }
}

bool Button::clicked(sf::RenderWindow& win, const sf::Event& ev) {
    // Check for a Mouse Button Pressed event specifically for the Left click
    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left) {
        
        // Convert the event's pixel coordinates to a floating-point vector
        sf::Vector2f mousePos((float)ev.mouseButton.x, (float)ev.mouseButton.y);
        
        // Return true if the click happened within the button's hitbox
        return box.getGlobalBounds().contains(mousePos);
    }
    return false;
}

void Button::draw(sf::RenderWindow& win) {
    win.draw(box);  
    win.draw(text); 
}

sf::Vector2f Button::getPosition() const {
    return box.getPosition();
}

sf::FloatRect Button::getGlobalBounds() const {
    return box.getGlobalBounds();
}