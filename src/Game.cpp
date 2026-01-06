#include "Game.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/PauseMenu.hpp"
#include "Menus/MenuMapEditor.hpp"
const int TILE_SIZE = 40;

Game::Game()
    : mWindow(sf::VideoMode(800, 600), "Tower Defense - Modular"),
      mSpawnInterval(2.f),
      mSpawnTimer(0.f),
      mSpawnedCount(0),
      mMaxSpawn(20),
      mGameState(MenuState::MainMenu)
{
    mWindow.setFramerateLimit(60);

    mPath = {
        {50, 300}, {200, 300}, {200, 120},
        {500, 120}, {500, 450}, {750, 450}
    };

    mCurrentMenu = std::make_unique<MainMenu>(mWindow);

    mTowers.push_back(std::make_unique<Tower>(sf::Vector2f(2 * TILE_SIZE, 2 * TILE_SIZE), "assets/base_tower.png"));
    mTowers.push_back(std::make_unique<Tower>(sf::Vector2f(7 * TILE_SIZE, 5 * TILE_SIZE), "assets/base_tower.png"));
    
}


void Game::run() {
    sf::Clock clock;

    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event ev;
        while (mWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                mWindow.close();
            } else {
                if (mCurrentMenu) {
                    mCurrentMenu->handleEvent(ev, mWindow);
                } else {
                    processEvents(ev);
                }
            }
        }

        if (mCurrentMenu) {
            mCurrentMenu->update(dt, mWindow);

            MenuState nextState = mCurrentMenu->getNextState();
            if (nextState != MenuState::None && nextState != mGameState) {
                mGameState = nextState;

                switch (mGameState) {
                    case MenuState::MainMenu:
                        mCurrentMenu = std::make_unique<MainMenu>(mWindow);
                        break;
                    case MenuState::Pause:
                        mCurrentMenu = std::make_unique<PauseMenu>(mWindow);
                        break;
                    case MenuState::MapEditor:
                        mCurrentMenu = std::make_unique<MenuMapEditor>(mWindow);
                        break;
                    case MenuState::Gameplay:
                        mCurrentMenu.reset(); // Sai do menu, começa o jogo
                        break;
                    default:
                        break;
                }
            }
        } else {
            update(dt);
        }

        render();
    }
}

void Game::processEvents(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Escape) {
            mGameState = MenuState::Pause;
            mCurrentMenu = std::make_unique<PauseMenu>(mWindow);
        }
    }
}

void Game::update(float dt) {
    mSpawnTimer -= dt;
    if (mSpawnTimer <= 0.f && mSpawnedCount < mMaxSpawn) {
        // Corrigido: passou todos os parâmetros esperados no construtor Enemy
        mEnemies.push_back(std::make_unique<Enemy>(
            mPath.front(),              // posição inicial
            "assets/base_enemy.png",         // textura
            100,                       // vida
            10,                        // dano
            50.f,                      // velocidade
            20                         // recompensa
        ));

        mSpawnedCount++;
        mSpawnTimer = mSpawnInterval;
    }

    for (auto& enemy : mEnemies) {
        enemy->update(dt, mPath);
    }

    for (auto& tower : mTowers) {
        // Supondo que tower->update retorna um ponteiro ou std::unique_ptr<Projectile>
        auto proj = tower->update(dt, mEnemies);
        if (proj) {
            mProjectiles.push_back(std::move(*proj)); // Corrigido, se proj for unique_ptr, use std::move
        }
    }

    for (auto& proj : mProjectiles) {
        proj.update(dt);
    }

    handleCollisions();
}

void Game::handleCollisions() {
    for (auto& p : mProjectiles) {
        if (!p.isAlive()) continue;

        for (auto& e : mEnemies) {
            if (!e->isAlive()) continue;

            sf::Vector2f diff = e->getPosition() - p.getPosition();
            float distSq = diff.x * diff.x + diff.y * diff.y;

            if (distSq < 144.f) { // raio 12
                e->destroy();
                p.destroy();
                break;
            }
        }
    }

    mProjectiles.erase(std::remove_if(mProjectiles.begin(), mProjectiles.end(),
        [](const Projectile& p) { return !p.isAlive(); }), mProjectiles.end());

    mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }), mEnemies.end());
}

void Game::render() {
    mWindow.clear(sf::Color(30, 30, 30));

    if (!mPath.empty()) {
        std::vector<sf::Vertex> lines;
        for (const auto& pos : mPath)
            lines.emplace_back(pos, sf::Color::Green);
        mWindow.draw(&lines[0], lines.size(), sf::LineStrip);
    }

    for (auto& e : mEnemies)
        e->draw(mWindow);

    for (auto& t : mTowers)
        t->draw(mWindow);

    for (auto& p : mProjectiles)
        p.draw(mWindow);

    if (mCurrentMenu) {
        mCurrentMenu->draw(mWindow);
    }

    mWindow.display();
}
