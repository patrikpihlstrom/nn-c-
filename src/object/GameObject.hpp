#pragma once

#include "object/Object.hpp"


class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool isEntity() const;
	virtual bool isPlayerEntity() const;
	virtual bool isGameObject() const;
	virtual bool isLight() const;

private:
};

