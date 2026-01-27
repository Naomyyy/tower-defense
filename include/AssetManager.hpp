#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    // Singleton Pattern: Provides global access to the single instance
    static AssetManager& getInstance() {
        static AssetManager instance; // Initialized only once on the first call
        return instance;
    }

   // Loads a texture from a file and stores it in the map with a unique name
    void loadTexture(const std::string& name, const std::string& filename);

    // Returns a constant reference to a stored texture by its name
    const sf::Texture& getTexture(const std::string& name) const;

private:
    AssetManager() = default; 
    std::unordered_map<std::string, sf::Texture> mTextures;
};