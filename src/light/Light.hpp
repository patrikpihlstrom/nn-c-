#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "application/Math.hpp"

class Light
{
public:
	Light();
	~Light();

	void setTexture(const std::weak_ptr<sf::Texture> texture);
	void setBounds(const sf::Rect<float>& bounds);

	sf::Rect<float> getBounds() const;

private:
	sf::VertexArray m_vertexArray;
	std::weak_ptr<sf::Texture> m_texture;
	sf::Color m_color;
	float m_radius;
	std::vector<float> m_angles;
	std::vector<std::weak_ptr<math::Polygon>> m_objects;
	std::vector<sf::Vector2f> m_points;
	sf::Rect<float> m_bounds;
};

