#include "GameObject.hpp"


GameObject::GameObject() :
	Object()
{
	m_lightObstructor.reset(new LightObstructor());
}

GameObject::~GameObject()
{
}

void GameObject::update(const float& deltaTime)
{
	if (m_lightObstructor)
	{
		m_lightObstructor->setBoundingBox(m_boundingBox);
		m_lightObstructor->updateLightsVector();
		m_lightObstructor->notifyLights();
	}
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto blend = states.blendMode;
	states.blendMode = sf::BlendMode::BlendNone;
	target.draw(m_shape, states);
	states.blendMode = blend;
}

void GameObject::addLight(std::weak_ptr<Light> light)
{
	if (m_lightObstructor)
		m_lightObstructor->addLight(light);
}

void GameObject::clearLights()
{
	if (m_lightObstructor)
		m_lightObstructor->clearLights();
}

bool GameObject::isGameObject() const
{
	return true;
}

bool GameObject::isLight() const
{
	return false;
}

