#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Button : public sf::Drawable
{
public:
	Button();
	~Button();

	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	bool pressed;
	sf::Sprite m_sprite;

};

