#include "GameObject.hpp"


GameObject::GameObject() :
	Object()
{
}

GameObject::~GameObject()
{
}

void GameObject::clearShadow()
{
	m_shadow.clear();
}

void GameObject::buildShadow()
{
	m_shadowShape.setPointCount(m_shadow.getPointCount());
	m_shadowShape.setFillColor(sf::Color(30, 30, 30));

	for (int i = 0; i < m_shadow.getPointCount(); ++i)
		m_shadowShape.setPoint(i, m_shadow.getPoint(i));
}

void GameObject::accountForShadow(const math::Polygon& shadow)
{
	if (m_shadow.getPointCount() != 0)
	{
		m_shadow = m_shadow & shadow;
	}
	else
		m_shadow = shadow;

	m_shadow.constructEdges();
}

math::Polygon GameObject::getShadow() const
{
	return m_shadow;
}

void GameObject::drawShadow(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shadowShape, states);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
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

