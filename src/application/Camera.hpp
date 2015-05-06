#pragma once

#include <memory>

#include <SFML/Graphics/View.hpp>

#include "actor/Actor.hpp"


class Camera : public sf::View
{
public:
	Camera(const sf::Vector2f& center, const sf::Vector2f& dimensions);
	Camera();
	~Camera();

	void trackActor(const std::weak_ptr<Actor> actor);
	void update();

	void zoom(const float& factor);
	float getZoom() const;

private:
	std::weak_ptr<Actor> m_actor;
	sf::Vector2f m_velocity;

	float m_zoom;
};

