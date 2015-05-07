#pragma once

#include "object/Object.hpp"


class Light : public Object
{
public:
	Light();
	~Light();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	float getRadius() const;
	void setRadius(const float& radius);

	void clear();
	void accountForObject(const math::Polygon& polygon);
	void buildLightShape();

	void setTexture(const std::weak_ptr<sf::Texture> texutre);
	void setColor(const sf::Color& color);

	bool isEntity() const;
	bool isPlayerEntity() const;
	bool isGameObject() const;
	bool isLight() const;

	void update();

	void requireUpdate();
	bool requiresUpdate() const;

private:
	sf::Vector2f castRay(const float& angle);

	sf::VertexArray m_vertexArray;
	sf::Color m_color;
	std::vector<float> m_angles;
	std::vector<math::Polygon> m_objects;
	std::vector<sf::Vector2f> m_points;
	std::weak_ptr<sf::Texture> m_texture;
	sf::Vector2f m_min, m_max;
	math::Polygon m_bounds;

	float m_radius;

	bool m_requiresUpdate;
};

