#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "../map/Quadtree.hpp"
#include "../application/Math.hpp"


class ShadowCaster : public sf::Drawable
{
public:
	ShadowCaster();
	~ShadowCaster();

	void updateShape();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setQuadtree(const std::weak_ptr<Quadtree> quadtree);
	void setPosition(const sf::Vector2f& position);

	std::shared_ptr<math::Polygon> bounds;

private:
	std::vector<std::unique_ptr<sf::ConvexShape>> m_shadows;
	sf::Color m_color;
	sf::Vector2f m_position;

	std::weak_ptr<Quadtree> m_quadtree;
	std::shared_ptr<sf::Texture> m_texture;

	std::vector<std::weak_ptr<math::Polygon>> getPolygons(const std::vector<std::weak_ptr<Quadtree>>& quadtrees) const;
};

