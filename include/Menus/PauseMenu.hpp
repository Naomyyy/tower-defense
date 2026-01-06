#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class PauseMenu : public MenuScreen {
public:
    explicit PauseMenu(sf::RenderWindow& window);

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    MenuState getNextState() const override;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;

    std::unique_ptr<Button> mResumeButton;
    std::unique_ptr<Button> mMainMenuButton;

    MenuState mNextState = MenuState::None;
};
