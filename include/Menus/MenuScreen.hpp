#pragma once
#include <SFML/Graphics.hpp>

enum class MenuState {
    None,
    MainMenu,
    Pause,
    Gameplay,
    MapEditor,
    GameOver, 
    Win,
    Difficulty
};

class MenuScreen {
public:
    virtual ~MenuScreen() = default;
    virtual void handleEvent(const sf::Event& ev, sf::RenderWindow& window) = 0;
    virtual void update(float dt, sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& win) = 0;

    virtual MenuState getNextState() const { return MenuState::None; }
};
