#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../map/Quadtree.hpp"
#include "../application/Math.hpp"
#include "JumpCheck.hpp"


class Entity : public sf::Drawable
{
public:
	Entity();
	virtual ~Entity();

	void setQuadtree(const std::weak_ptr<Quadtree> quadtree);
	void update(const float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f getPosition() const;
	sf::Vector2f getVelocity() const;

	float getMaxHorizontalSpeed() const;

protected:
	virtual void control(const float deltaTime) = 0;

	math::Polygon m_polygon;
	sf::ConvexShape m_shape;

	sf::Vector2f m_velocity;

	float m_maxHorizontalSpeed;

	JumpCheck m_jumpChecks[3];

private:
	void collide(const math::Polygon& polygon);
	void checkCollisions();
	void move(const sf::Vector2f& offset);
	
	std::weak_ptr<Quadtree> m_quadtree;
};

