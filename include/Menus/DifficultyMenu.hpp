#ifndef DIFFICULTYMENU_HPP
#define DIFFICULTYMENU_HPP

#include "MenuScreen.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class DifficultyMenu : public MenuScreen {
public:
    explicit DifficultyMenu(sf::RenderWindow& window);
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) override;
    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    int getSelectedDifficulty() const;
    MenuState getNextState() const override;

private:
    sf::Font mFont;
    sf::Text mTitle;
    std::unique_ptr<Button> mEasyBtn;
    std::unique_ptr<Button> mNormalBtn;
    std::unique_ptr<Button> mHardBtn;
    MenuState mNextState = MenuState::None;
    int mSelection = 1; // 0: Easy, 1: Normal, 2: Hard

    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;
};

#endif