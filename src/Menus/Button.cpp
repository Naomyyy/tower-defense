#include "Menus/Button.hpp"

Button::Button(const std::string& label, sf::Vector2f pos, sf::Font& font, unsigned int size) {
    // 1. Configurar o Texto
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(normalColor); // Começa Branco

    // --- LÓGICA DE CENTRALIZAÇÃO DO TEXTO ---
    // Pega o retângulo do texto para achar o centro
    sf::FloatRect textRect = text.getLocalBounds();
    // Define a origem do texto no exato centro dele
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top  + textRect.height / 2.0f);
    // Posiciona o texto na coordenada passada
    text.setPosition(pos);
    // ----------------------------------------

    // 2. Configurar a Caixa (Hitbox invisível)
    // A caixa serve apenas para o mouse detectar o clique
    sf::Vector2f padding(20.f, 10.f); // Espaço extra para facilitar o clique
    box.setSize(sf::Vector2f(textRect.width + padding.x * 2, textRect.height + padding.y * 2));
    
    // Centraliza a caixa na mesma posição do texto
    box.setOrigin(box.getSize() / 2.0f);
    box.setPosition(pos);

    // DEIXA A CAIXA INVISÍVEL (Sem borda, sem fundo)
    box.setFillColor(sf::Color::Transparent); 
    // Se quiser ver a caixa para debug, descomente a linha abaixo:
    // box.setOutlineColor(sf::Color::Red); box.setOutlineThickness(1);
}

void Button::update(sf::RenderWindow& win) {
    sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(win);
    isHovered = box.getGlobalBounds().contains(mp);

    if (isHovered) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            text.setFillColor(pressedColor); // Cor ao clicar
        } else {
            text.setFillColor(hoverColor);   // Cor ao passar o mouse (Verde)
        }
    } else {
        text.setFillColor(normalColor);      // Cor normal (Branco)
    }
}

bool Button::clicked(sf::RenderWindow& win, const sf::Event& ev) {
    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left) {
        
        sf::Vector2f mp((float)ev.mouseButton.x, (float)ev.mouseButton.y);
        return box.getGlobalBounds().contains(mp);
    }
    return false;
}

void Button::draw(sf::RenderWindow& win) {
    win.draw(box);  // Desenha a caixa (invisível, mas necessária se tiver debug)
    win.draw(text); // Desenha o texto por cima
}

sf::Vector2f Button::getPosition() const {
    return box.getPosition();
}

sf::FloatRect Button::getGlobalBounds() const {
    return box.getGlobalBounds();
}