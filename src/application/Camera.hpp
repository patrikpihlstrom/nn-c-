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

	template <typename T>
	sf::Rect<T> getBounds() const
	{
		return sf::Rect<T>{(T)(getCenter().x - getSize().x/2), (T)(getCenter().y - getSize().y/2), (T)(getSize().x), (T)(getSize().y)};
	}

private:
	std::weak_ptr<Actor> m_actor;
	sf::Vector2f m_velocity;

	float m_zoom;
};

