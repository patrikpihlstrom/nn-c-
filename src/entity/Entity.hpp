#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../map/Quadtree.hpp"
#include "../application/Math.hpp"
#include "Units.hpp"


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
	virtual void control() = 0;
	virtual void entitySpecificMovement(const sf::Vector2f& offset);
	virtual void checkCollisions();
	virtual void terminalVelocity();

	math::Polygon m_polygon;
	sf::ConvexShape m_shape;

	sf::Vector2f m_velocity, m_acceleration;
	float m_maxHorizontalSpeed;

	std::weak_ptr<Quadtree> m_quadtree;
	
private:
	void move(const sf::Vector2f& offset);
};

