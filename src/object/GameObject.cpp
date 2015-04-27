#include "GameObject.hpp"


GameObject::GameObject() :
	Object()
{
}

GameObject::~GameObject()
{
}

void GameObject::update(const float& deltaTime)
{
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto blend = states.blendMode;
	states.blendMode = sf::BlendMode::BlendNone;
	target.draw(m_shape, states);
	states.blendMode = blend;
}

bool GameObject::isEntity() const
{
	return false;
}

bool GameObject::isPlayerEntity() const
{
	return false;
}

bool GameObject::isGameObject() const
{
	return true;
}

bool GameObject::isLight() const
{
	return false;
}

