#pragma once

#include <SFML/Graphics.hpp>


class Actor : public sf::Transformable, public sf::Drawable
{
public:
	Actor();
	~Actor();

	uint8_t getHealth() const;
	void setHealth(const uint8_t& health);
	void damage(const int8_t& factor);

private:
	uint8_t m_health;

};

