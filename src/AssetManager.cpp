#include "AssetManager.hpp"
#include <iostream>
#include <utility> 

void AssetManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (texture.loadFromFile(filename)) {
        mTextures[name] = std::move(texture); 
    } else {
        std::cerr << "ERRO: Nao foi possivel carregar a textura: " << filename << std::endl;
    }
}

const sf::Texture& AssetManager::getTexture(const std::string& name) const {
    auto found = mTextures.find(name);
    if (found == mTextures.end()) {
        std::cerr << "ERRO: Textura '" << name << "' nao encontrada." << std::endl;
        throw std::runtime_error("Textura nao encontrada!"); 
    }
    return found->second;
}