#pragma once

#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Button : public sf::Drawable
{
public:
	Button();
	~Button();

	void update(const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool pressed() const;

	void setSprite(const sf::Sprite& sprite);

private:
	bool m_pressed;
	sf::Sprite m_sprite;
};

