#include "Menus/MenuMapEditor.hpp"

MapEditor::MapEditor(sf::RenderWindow& window, int tileSize)
    : mWindow(window), mTileSize(tileSize) 
{
    mGridWidth = window.getSize().x / tileSize;
    mGridHeight = window.getSize().y / tileSize;

    // Inicializa grid vazia
    mWalls.resize(mGridHeight, std::vector<bool>(mGridWidth, false));
}

void MapEditor::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::Closed) {
        mWindow.close();
    }

    if (ev.type == sf::Event::KeyPressed) {
        switch (ev.key.code) {
            case sf::Keyboard::Escape:
                mNextState = MenuState::MainMenu;
                break;
            case sf::Keyboard::S:
                saveToFile("map.txt");
                break;
            case sf::Keyboard::L:
                loadFromFile("map.txt");
                break;
            case sf::Keyboard::Num1:
                mCurrentMode = PlacementMode::Wall;
                break;
            case sf::Keyboard::Num2:
                mCurrentMode = PlacementMode::Erase;
                break;
            default:
                break;
        }
    }

    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        mMouseDown = true;
    }
    if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left) {
        mMouseDown = false;
    }
}

void MapEditor::update(float dt) {
    if (!mMouseDown) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    int tileX = mousePos.x / mTileSize;
    int tileY = mousePos.y / mTileSize;

    if (tileX >= 0 && tileX < mGridWidth && tileY >= 0 && tileY < mGridHeight) {
        if (mCurrentMode == PlacementMode::Wall)
            mWalls[tileY][tileX] = true;
        else if (mCurrentMode == PlacementMode::Erase)
            mWalls[tileY][tileX] = false;
    }
}

void MapEditor::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    sf::RectangleShape tileShape(sf::Vector2f((float)mTileSize - 1.f, (float)mTileSize - 1.f));
    tileShape.setOutlineThickness(1.f);
    tileShape.setOutlineColor(sf::Color(50, 50, 50));

    for (int y = 0; y < mGridHeight; ++y) {
        for (int x = 0; x < mGridWidth; ++x) {
            tileShape.setPosition((float)(x * mTileSize), (float)(y * mTileSize));
            if (mWalls[y][x]) {
                tileShape.setFillColor(sf::Color::White);
            } else {
                tileShape.setFillColor(sf::Color::Black);
            }
            window.draw(tileShape);
        }
    }

    window.display();
}

MenuState MapEditor::getNextState() const {
    return mNextState;
}

void MapEditor::saveToFile(const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Erro ao salvar mapa\n";
        return;
    }

    for (int y = 0; y < mGridHeight; ++y) {
        for (int x = 0; x < mGridWidth; ++x) {
            ofs << (mWalls[y][x] ? '1' : '0');
        }
        ofs << '\n';
    }

    std::cout << "Mapa salvo em " << filename << std::endl;
}

void MapEditor::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Erro ao carregar mapa\n";
        return;
    }

    std::string line;
    int y = 0;
    while (std::getline(ifs, line) && y < mGridHeight) {
        for (int x = 0; x < (int)line.size() && x < mGridWidth; ++x) {
            mWalls[y][x] = (line[x] == '1');
        }
        ++y;
    }

    std::cout << "Mapa carregado de " << filename << std::endl;
}
