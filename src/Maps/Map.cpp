#include "Maps/Map.hpp"

Map::Map() {
    // Configura visual básico (depois usamos texturas)
    mGrassShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    mGrassShape.setFillColor(sf::Color(34, 139, 34)); // Verde Floresta

    mRoadShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    mRoadShape.setFillColor(sf::Color(194, 178, 128)); // Cor de areia/estrada
    
    load();
}

void Map::load() {
    // 0 = Grama, 1 = Estrada
    // Desenhe o caminho alterando os 0s para 1s
    int tempMap[GRID_HEIGHT][GRID_WIDTH] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Começa na esquerda
        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1}, // Sai na direita
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    // Copia para a variável da classe
    for(int y=0; y<GRID_HEIGHT; ++y) {
        for(int x=0; x<GRID_WIDTH; ++x) {
            mGrid[y][x] = tempMap[y][x];
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for(int y=0; y<GRID_HEIGHT; ++y) {
        for(int x=0; x<GRID_WIDTH; ++x) {
            // Calcula a posição na tela (x * 40, y * 40)
            sf::Vector2f pos(x * TILE_SIZE, y * TILE_SIZE);
            
            if (mGrid[y][x] == 0) {
                mGrassShape.setPosition(pos);
                window.draw(mGrassShape);
            } else if (mGrid[y][x] == 1) {
                mRoadShape.setPosition(pos);
                window.draw(mRoadShape);
            }
        }
    }
}

int Map::getTileId(float mouseX, float mouseY) {
    // Converte pixel mouse -> indice da matriz
    int col = (int)(mouseX / TILE_SIZE);
    int row = (int)(mouseY / TILE_SIZE);

    // Proteção para não clicar fora do mapa e travar o jogo
    if (col < 0 || col >= GRID_WIDTH || row < 0 || row >= GRID_HEIGHT) 
        return -1; // -1 significa fora do mapa

    return mGrid[row][col];
}