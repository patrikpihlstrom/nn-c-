#pragma once

#include <memory>
#include <map>
#include <fstream>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>


class TextureHolder
{
public:
	TextureHolder();
	~TextureHolder();

	void loadTextures(const std::string& path);
	std::weak_ptr<sf::Texture> getTexture(const std::string& key) const;

private:
	void loadTexture(const std::string& line);
	std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
};

