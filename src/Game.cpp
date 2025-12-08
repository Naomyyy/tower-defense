#include "Game.hpp"
#include "AssetManager.hpp"
#include <memory>


Game::Game()
    : mWindow(sf::VideoMode(800, 600), "Tower Defense - Modular"),
      mGameState(MenuState::MainMenu)
{
    mWindow.setFramerateLimit(60);

    //Carrega as texturas
    AssetManager& assets = AssetManager::getInstance();
    assets.loadTexture("base_enemy", "assets/base_enemy.png");
    assets.loadTexture("base_tower", "assets/base_tower.png");

    // Inicializa o menu principal
    mCurrentMenu = std::make_unique<MainMenu>();
}

void Game::run() {
    sf::Clock clock;

    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event ev;
        while (mWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                mWindow.close();

            if (mCurrentMenu) {
                mCurrentMenu->handleEvent(ev);
            } else {
                processEvents(ev);
            }
        }

        if (mCurrentMenu) {
            mCurrentMenu->update(dt);

            MenuState next = mCurrentMenu->getNextState();
            if (next != MenuState::None) {
                mGameState = next;

                switch (mGameState) {
                    case MenuState::Gameplay:
                        mCurrentMenu.reset(); // sai do menu e começa o jogo
                        break;
                    case MenuState::Pause:
                        mCurrentMenu = std::make_unique<PauseMenu>();
                        break;
                    case MenuState::MapEditor:
                        mCurrentMenu = std::make_unique<MapEditor>(mWindow, 20);
                        break;
                    case MenuState::MainMenu:
                        mCurrentMenu = std::make_unique<MainMenu>();
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
    // Exemplo: tecla ESC para pausar o jogo
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        mGameState = MenuState::Pause;
        mCurrentMenu = std::make_unique<PauseMenu>();
    }
    else if(ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left){
        sf::Vector2i mousePos = {ev.mouseButton.x, ev.mouseButton.y};
        // Converte as coordenadas do pixel para a posição X, Y do tile
        int tileX = mousePos.x / TILE_SIZE;
        int tileY = mousePos.y / TILE_SIZE;
        // Verifica se as coordenadas do tile estão dentro dos limites do mapa e adiciona a torre
        if (tileX >= 0 && tileX < GRID_WIDTH && tileY >= 0 && tileY < GRID_HEIGHT) {
            if (mMap.getTileId(tileX, tileY) == 0) {
                float centerX = tileX * TILE_SIZE + TILE_SIZE / 2.f;
                float centerY = tileY * TILE_SIZE + TILE_SIZE / 2.f;
                mTowers.push_back(std::make_unique<Tower>(sf::Vector2f{centerX, centerY}, "base_tower"));
            }
        }
    }
}

void Game::update(float dt) {
    // 1. Spawner de inimigos
    mSpawnTimer -= dt;
    if (mSpawnTimer <= 0.f && mSpawnedCount < mMaxSpawn) {
        mEnemies.push_back(std::make_unique<Enemy>(mMap.getPath().front(), "base_enemy"));
        mSpawnedCount++;
        mSpawnTimer = mSpawnInterval;
    }

    // 2. Atualiza inimigos
    for (auto& enemy : mEnemies) {
        enemy->update(dt, mMap.getPath());
    }

    // 3. Torres tentam atirar
    for (auto& tower : mTowers) {
        if (auto proj = tower->update(dt, mEnemies)) {
            mProjectiles.push_back(*proj);
        }
    }

    // 4. Atualiza projéteis
    for (auto& proj : mProjectiles) {
        proj.update(dt);
    }

    // 5. Colisões e limpeza
    handleCollisions();
}

void Game::handleCollisions() {
    for (auto& p : mProjectiles) {
        if (!p.isAlive()) continue;

        for (auto& e : mEnemies) {
            if (!e->isAlive()) continue;

            sf::Vector2f diff = e->getPosition() - p.getPosition();
            float distSq = diff.x * diff.x + diff.y * diff.y;

            if (distSq < 144.f) { // 12 * 12
                e->destroy();
                p.destroy();
                break;
            }
        }
    }

    // Remove projéteis mortos
    mProjectiles.erase(std::remove_if(mProjectiles.begin(), mProjectiles.end(),
                                      [](const Projectile& p) { return !p.isAlive(); }),
                       mProjectiles.end());

    // Remove inimigos mortos
    mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
                                  [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }),
                   mEnemies.end());
}

void Game::render() {
    mWindow.clear(sf::Color(30, 30, 30));

    if (mCurrentMenu) {
        mCurrentMenu->draw(mWindow);
    } else {
        mMap.draw(mWindow);
        if (!mPath.empty()) {
            std::vector<sf::Vertex> lines;
            for (const auto& pos : mPath)
                lines.emplace_back(pos, sf::Color::Green);
            mWindow.draw(&lines[0], lines.size(), sf::LineStrip);
        }

        for (auto& e : mEnemies) e->draw(mWindow);
        for (auto& t : mTowers) t->draw(mWindow);
        for (auto& p : mProjectiles) p.draw(mWindow);
    }

    mWindow.display();
}
