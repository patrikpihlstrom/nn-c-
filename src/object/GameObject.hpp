#pragma once

#include "object/Object.hpp"
#include "object/Light.hpp"
#include "object/LightObstructor.hpp"


class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void addLight(std::weak_ptr<Light> light);
	void clearLights();

	virtual bool isGameObject() const;
	virtual bool isLight() const;

private:
	std::shared_ptr<LightObstructor> m_lightObstructor;
};

