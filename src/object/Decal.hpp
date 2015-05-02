#pragma once

#include "object/Object.hpp"


class Decal : public Object
{
public:
	Decal();
	~Decal();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void rotate(const float& angle);
	void setColor(const sf::Color& color);

	virtual bool isEntity() const;
	virtual bool isPlayerEntity() const;
	virtual bool isGameObject() const;
	virtual bool isLight() const;

private:
	sf::Color m_color;
};

