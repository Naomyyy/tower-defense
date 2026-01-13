#include "Game.hpp"
#include "AssetManager.hpp"
#include <iostream>
#include <algorithm> // Para std::remove_if

// Inclua os headers das suas torres. 
// Se estiverem todos em Tower.hpp, basta ele.
#include "Tower.hpp" 

// Definições de tamanho (se não estiverem em um header de constantes global, defina aqui ou inclua o header)
// Vou assumir valores padrão caso você não tenha um header "Constants.hpp"
#ifndef TILE_SIZE
#define TILE_SIZE 64 // Ajuste para o tamanho do seu tile real
#define GRID_WIDTH 20
#define GRID_HEIGHT 15
#endif

Game::Game()
    : mWindow(sf::VideoMode(800, 600), "Tower Defense - Modular"),
      mSpawnInterval(2.f),
      mSpawnTimer(0.f),
      mSpawnedCount(0),
      mMaxSpawn(20),
      mGameState(MenuState::MainMenu),
      mSelectedTower(TowerType::Archer) // Começa com Arqueiro selecionado por padrão
{
    mWindow.setFramerateLimit(60);

    // --- CARREGAMENTO DE ASSETS ---
    AssetManager& assets = AssetManager::getInstance();
    
    // Inimigos e UI
    assets.loadTexture("base_enemy", "assets/golem.png");
    
    // Torres (Certifique-se que essas imagens existem na pasta assets!)
    assets.loadTexture("base_tower",   "assets/base_tower.png");   // Fallback
    assets.loadTexture("archer_tower", "assets/torre1.png"); // Arqueiro
    assets.loadTexture("cannon_tower", "assets/torre2.png"); // Canhão
    assets.loadTexture("mage_tower",   "assets/torre3.png");   // Mago

    // Projéteis
    assets.loadTexture("arrow",        "assets/stone.png");
    assets.loadTexture("cannonball",   "assets/lightning.png");
    assets.loadTexture("magic_bolt",   "assets/barrel.png");

    // Inicializa o menu principal
    mCurrentMenu = std::make_unique<MainMenu>(mWindow);
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
                        mMap.load();
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
    // Tecla ESC para pausar
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Escape) {
            mGameState = MenuState::Pause;
            mCurrentMenu = std::make_unique<PauseMenu>(mWindow);
        }
        // --- SELEÇÃO DE TORRES (Teclas 1, 2, 3) ---
        else if (ev.key.code == sf::Keyboard::Num1) {
            mSelectedTower = TowerType::Archer;
            std::cout << "Selecionado: Arqueiro" << std::endl;
        }
        else if (ev.key.code == sf::Keyboard::Num2) {
            mSelectedTower = TowerType::Cannon;
            std::cout << "Selecionado: Canhao" << std::endl;
        }
        else if (ev.key.code == sf::Keyboard::Num3) {
            mSelectedTower = TowerType::Mage;
            std::cout << "Selecionado: Mago" << std::endl;
        }
    }
    // --- COLOCAR TORRE (Clique) ---
    else if(ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left){
        sf::Vector2i mousePos = {ev.mouseButton.x, ev.mouseButton.y};
        
        // Converte pixel -> grid
        int tileX = mousePos.x / TILE_SIZE;
        int tileY = mousePos.y / TILE_SIZE;

        // Verifica limites do mapa
        if (tileX >= 0 && tileX < GRID_WIDTH && tileY >= 0 && tileY < GRID_HEIGHT) {
            // Só constrói se o tile estiver livre (ID 0)
            if (mMap.getTileId(tileX, tileY) == 0) {
                float centerX = tileX * TILE_SIZE + TILE_SIZE / 2.f;
                float centerY = tileY * TILE_SIZE + TILE_SIZE / 2.f;
                sf::Vector2f towerPos = {centerX, centerY};

                // --- FÁBRICA DE TORRES ---
                // Cria a torre baseada na seleção atual
                switch (mSelectedTower) {
                    case TowerType::Archer:
                        mTowers.push_back(std::make_unique<ArcherTower>(towerPos));
                        break;
                    case TowerType::Cannon:
                        mTowers.push_back(std::make_unique<CannonTower>(towerPos));
                        break;
                    case TowerType::Mage:
                        mTowers.push_back(std::make_unique<MageTower>(towerPos));
                        break;
                }
            }
        }
    }
}

void Game::update(float dt) {
    // 1. Spawner de inimigos
    mSpawnTimer -= dt;
    
    const std::vector<sf::Vector2f>& path = mMap.getPath();

    if (mSpawnTimer <= 0.f && mSpawnedCount < mMaxSpawn) {
        if (!path.empty()) {
            mEnemies.push_back(std::make_unique<Enemy>(
                path.front(), 
                "base_enemy", 
                100, 10, 60.f, 20
            ));
            mSpawnedCount++;
            mSpawnTimer = mSpawnInterval;
        } 
    }

    // 2. Atualiza inimigos
    for (auto& enemy : mEnemies) {
        enemy->update(dt, mMap.getPath());
    }

    // 3. Torres tentam atirar
    for (auto& tower : mTowers) {
        // Agora o update retorna std::optional<Projectile> configurado com dano/velocidade da torre
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

            // Raio de colisão simples (12px)
            if (distSq < 144.f) { 
                // O projétil agora carrega seu próprio dano!
                e->takeDamage(p.getDamage()); 
                p.destroy();
                break; // Um projétil acerta apenas um inimigo (a menos que seja explosivo)
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
        // 1. Desenha Mapa
        mMap.draw(mWindow);
        
        // Debug do caminho (opcional)
        if (!mPath.empty()) {
            std::vector<sf::Vertex> lines;
            for (const auto& pos : mPath)
                lines.emplace_back(pos, sf::Color::Green);
            // mWindow.draw(&lines[0], lines.size(), sf::LineStrip);
        }

        // 2. Desenha Entidades
        for (auto& e : mEnemies) e->draw(mWindow);
        for (auto& t : mTowers) t->draw(mWindow);
        for (auto& p : mProjectiles) p.draw(mWindow);

        // 3. HUD / Feedback Visual de Seleção
        // Mostra uma bolinha ao lado do mouse indicando qual torre está selecionada
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        sf::CircleShape selectorIcon(8);
        selectorIcon.setPosition(static_cast<float>(mousePos.x) + 15, static_cast<float>(mousePos.y) + 15);
        
        if (mSelectedTower == TowerType::Archer) selectorIcon.setFillColor(sf::Color::Green);     // Arqueiro = Verde
        else if (mSelectedTower == TowerType::Cannon) selectorIcon.setFillColor(sf::Color::Red);  // Canhão = Vermelho
        else if (mSelectedTower == TowerType::Mage) selectorIcon.setFillColor(sf::Color::Blue);   // Mago = Azul
        
        mWindow.draw(selectorIcon);
    }

    mWindow.display();
}