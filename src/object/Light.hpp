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

	bool isGameObject() const;
	bool isLight() const;

	void update();

	void requestUpdate();
	bool requiresUpdate() const;

private:
	sf::Vector2f castRay(const float& angle);

	sf::VertexArray m_vertexArray;
	std::weak_ptr<sf::Texture> m_texture;
	sf::Color m_color;

	std::vector<math::Polygon> m_objects; // Used to check collisions of rays
	std::vector<sf::Vector2f> m_points; // Store the points in order to build the shape
	std::vector<float> m_angles; // Use to cast rays in sorted manner

	float m_radius;
	bool m_requiresUpdate;
};

