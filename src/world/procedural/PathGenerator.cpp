#include "world/procedural/PathGenerator.hpp"


PathGenerator::PathGenerator()
{
}

PathGenerator::~PathGenerator()
{
}

void PathGenerator::setTexture(const std::weak_ptr<sf::Texture> texture)
{
	m_texture = texture;
}

Path PathGenerator::generatePath(const sf::Vector2f& origin, const sf::Vector2f& destination, const uint8_t& resolution, const float& maxDeviation)
{
	Path path;
	path.addPoint(origin);

	sf::Vector2f direction = {destination.x - origin.x, destination.y - origin.y};
	math::normalize(direction);

	float step = math::distance<float>(origin, destination)/resolution;

	for (int i = 0; i < resolution - 1; ++i)
	{
		auto point = sf::Vector2f(origin.x + (i + 1)*direction.x*step, origin.y + (i + 1)*direction.y*step);
		math::rotateVector<float>(i == 0 ? origin:path.getPoint(i - 1), point, (float)(rand()%(int)(maxDeviation*200))/100 - maxDeviation);
		path.addPoint(point);
	}

	path.addPoint(destination);
	path.build(m_texture);

	return path;
}

