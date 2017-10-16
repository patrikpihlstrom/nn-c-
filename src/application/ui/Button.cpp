#include "Button.hpp"
#include <iostream>


Button::Button() :
	m_pressed(false)
{
}

Button::~Button()
{
}

void Button::update(const sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (sf::Rect<int>(m_sprite.getPosition().x, m_sprite.getPosition().y, m_sprite.getTextureRect().width, m_sprite.getTextureRect().height).intersects({sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 1, 1}))
			m_pressed = true;
		else
			m_pressed = false;
	}
	else
		m_pressed = false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

bool Button::pressed() const
{
	return m_pressed;
}

void Button::setSprite(const sf::Sprite& sprite)
{
	m_sprite = sprite;
}

void Button::setColor(const sf::Color& color)
{
	m_sprite.setColor(color);
}

