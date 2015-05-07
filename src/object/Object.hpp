#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

#include "object/LightObstructor.hpp"
#include "object/ObjectIdTracker.hpp"
#include "application/Math.hpp"


class Object : public sf::Drawable, public sf::Transformable
{
public:
	Object();
	Object(const ObjectId& id);
	virtual ~Object();

	void assign(const ObjectId& id);
	ObjectId getId() const;

	sf::FloatRect getGlobalBounds() const;
	sf::FloatRect getLocalBounds() const;

	sf::Rect<int> getBoundingBox() const;

	void setPolygon(const math::Polygon& polygon);
	math::Polygon getPolygon() const;

	void updateShape();
	virtual void setTexture(const std::weak_ptr<sf::Texture> texture);
	void setTextureRect(const sf::Rect<int>& rect);

	void move(const float x, const float y);

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool isEntity() const = 0;
	virtual bool isPlayerEntity() const = 0;
	virtual bool isGameObject() const = 0;
	virtual bool isLight() const = 0;

	void setSolid(const bool solid);
	bool getSolid() const;

	void generateLightObstructer();
	void addLight(std::weak_ptr<Light> light);
	void removeLight(const ObjectId& id);
	void clearLights();

protected:
	ObjectId m_id;
	math::Polygon m_polygon;
	sf::Rect<int> m_boundingBox;
	sf::Vector2f m_position;
	bool m_solid;

	sf::ConvexShape m_shape;

	virtual void control(const float& deltaTime);

	std::unique_ptr<LightObstructor> m_lightObstructor;
};

