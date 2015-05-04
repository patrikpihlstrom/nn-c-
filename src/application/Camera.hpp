#pragma once

#include <SFML/Graphics/View.hpp>


class Camera : public sf::View
{
public:
	Camera(const sf::Vector2f& center, const sf::Vector2f& dimensions);
	Camera();
	~Camera();

	void update();

	void zoom(const float& factor);
	float getZoom() const;

private:
	sf::Vector2f m_velocity;

	float m_zoom;
};

