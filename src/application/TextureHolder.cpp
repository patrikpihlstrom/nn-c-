#include "TextureHolder.hpp"


TextureHolder::TextureHolder()
{
}

TextureHolder::~TextureHolder()
{
}

std::weak_ptr<sf::Texture> TextureHolder::getTexture(const std::string& key) const
{
	return m_textures.find(key)->second;
}

void TextureHolder::loadTextures(const std::string& path)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		std::string line, key, _path;

		while (!file.eof())
		{
			std::getline(file, line);

			if (line.size() >= 10)
			{
				if (line.substr(0, 6) == "newmtl")
				{
					key = line.substr(7, line.size() - 1);
				}
				else if (line.substr(0, 6) == "map_Kd")
				{
					_path = line.substr(7, line.size() - 1);
					loadTexture(key + '=' + _path);
				}
			}
		}

		file.close();
	}
	else
	{
		std::cout << "Unable to open texture file." << std::endl;
	}
}

void TextureHolder::loadTexture(const std::string& line)
{
	sf::Image image;
	std::string path = std::string(line.substr(line.find('=') + 1));
	if (!image.loadFromFile(path))
	{
		std::cout << "Failed to load image. \"" << path << '\"' << std::endl;
		return;
	}

	auto key = line.substr(0, line.find('='));
	m_textures[key].reset(new sf::Texture());
	m_textures[key]->loadFromImage(image, sf::IntRect(0, 0, image.getSize().x, image.getSize().y));
	m_textures[key]->setRepeated(true);
}

