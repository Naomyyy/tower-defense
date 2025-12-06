// MenuScreen.hpp
#pragma once
#include <SFML/Graphics.hpp>

enum class MenuState {
    None,
    MainMenu,
    Pause,
    Gameplay,
    MapEditor
};

class MenuScreen {
public:
    virtual ~MenuScreen() = default;

    virtual void handleEvent(const sf::Event& ev) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& win) = 0;

    virtual MenuState getNextState() const { return MenuState::None; }
};
