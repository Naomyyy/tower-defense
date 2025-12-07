#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    // Padrão Singleton: Acesso global à única instância
    static AssetManager& getInstance() {
        static AssetManager instance; // Criada na primeira chamada
        return instance;
    }

    // Carrega uma textura de um arquivo e a armazena
    void loadTexture(const std::string& name, const std::string& filename);

    // Retorna uma referência à textura armazenada
    const sf::Texture& getTexture(const std::string& name) const;

private:
    AssetManager() = default; 
    std::unordered_map<std::string, sf::Texture> mTextures;
};