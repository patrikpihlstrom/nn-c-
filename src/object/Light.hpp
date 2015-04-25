#pragma once

#include "Object.hpp"


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

private:
	sf::Vector2f castRay(const float& angle);

	sf::VertexArray m_vertexArray;
	sf::Color m_color;
	std::vector<float> m_angles;
	std::vector<math::Segment<float>> m_objects;
	std::vector<sf::Vector2f> m_points;

	std::weak_ptr<sf::Texture> m_texture;

	float m_radius;
};

