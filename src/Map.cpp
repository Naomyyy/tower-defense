#include "Map.hpp"

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

    mPath.clear();
    int currentX = -1;
    int currentY = -1;
    
    // 1. Encontrar o Ponto de Partida (Primeira Coluna onde a estrada começa)
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        if (mGrid[y][0] == 1) {
            currentX = 0;
            currentY = y;
            break;
        }
    }
    if (currentX == -1) return; 

    // O ponto de partida é sempre um waypoint
    mPath.push_back(getCenterCoords(currentX, currentY));
    
    // Marca o tile inicial como visitado ('2')
    mGrid[currentY][currentX] = 2; 
    
    // Assumimos que o movimento inicial é para a direita (dx=1, dy=0)
    int lastDirX = 1; 
    int lastDirY = 0; 
    
    // Array de vizinhos: [dy, dx]
    int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Direita, Esquerda, Baixo, Cima

    // 2. Rastreamento e Registro de Curvas
    while (true) {
        bool foundNext = false;
        int nextX = -1, nextY = -1;
        int newDirX = 0, newDirY = 0;

        // Procura o próximo tile ('1')
        for (int i = 0; i < 4; ++i) {
            int checkX = currentX + dir[i][1];
            int checkY = currentY + dir[i][0];

            if (checkX >= 0 && checkX < GRID_WIDTH &&
                checkY >= 0 && checkY < GRID_HEIGHT &&
                mGrid[checkY][checkX] == 1) 
            {
                // Encontrou o próximo tile válido
                nextX = checkX;
                nextY = checkY;
                newDirX = dir[i][1];
                newDirY = dir[i][0];
                foundNext = true;
                break; 
            }
        }

        if (!foundNext) {
            // FIM DO CAMINHO: Registra o waypoint final e sai
            mPath.push_back(getCenterCoords(currentX, currentY));
            break; 
        }

        // Se a direção de movimento mudou, o ponto ATUAL é uma curva (Waypoint)
        if (newDirX != lastDirX || newDirY != lastDirY) {
            mPath.push_back(getCenterCoords(currentX, currentY));
            
            // Atualiza a direção
            lastDirX = newDirX;
            lastDirY = newDirY;
        }

        // Movimento para o próximo tile
        currentX = nextX;
        currentY = nextY;
        mGrid[currentY][currentX] = 2; // Marca como visitado (não será encontrado novamente)
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

const std::vector<sf::Vector2f>& Map::getPath() const{return mPath;}

sf::Vector2f Map::getCenterCoords(int x, int y) const {
    const float TILE_CENTER = TILE_SIZE / 2.f;
    return sf::Vector2f(x * TILE_SIZE + TILE_CENTER, y * TILE_SIZE + TILE_CENTER);
}