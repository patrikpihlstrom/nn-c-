#pragma once

#include "Object.hpp"


class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	void clearShadow();
	void buildShadow();
	void accountForShadow(const math::Polygon& shadow);
	math::Polygon getShadow() const;

	void drawShadow(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool isEntity() const;
	virtual bool isPlayerEntity() const;
	virtual bool isGameObject() const;
	virtual bool isLight() const;

private:
	math::Polygon m_shadow;
	sf::ConvexShape m_shadowShape;
};

