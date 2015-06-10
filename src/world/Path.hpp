#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>


class Path : public sf::Drawable
{
public:
	Path();
	~Path();

	void setPoint(const uint8_t& index, const sf::Vector2f& point);
	sf::Vector2f getPoint(const uint8_t& index) const;

	void addPoint(const sf::Vector2f& point);

	sf::Vector2f getFirst() const;
	sf::Vector2f getLast() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray m_vertexArray;
};

