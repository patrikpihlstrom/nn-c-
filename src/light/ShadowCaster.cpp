#include "ShadowCaster.hpp"


ShadowCaster::ShadowCaster()
{
	m_bounds.reset(new math::Polygon());
	m_bounds->addPoint(sf::Vector2f(0, 0));
	m_bounds->addPoint(sf::Vector2f(1600, 0));
	m_bounds->addPoint(sf::Vector2f(1600, 1200));
	m_bounds->addPoint(sf::Vector2f(0, 1200));
	m_bounds->constructEdges();
}

ShadowCaster::~ShadowCaster()
{
}

void ShadowCaster::setQuadtree(const std::weak_ptr<Quadtree> quadtree)
{
	m_quadtree = quadtree;
}

void ShadowCaster::updateShape()
{
	m_shadows.clear();
	if (auto quadtree = m_quadtree.lock())
	{
		std::vector<std::weak_ptr<Quadtree>> quadtrees;
		quadtree->getQuadtrees(quadtrees, *m_bounds);
		
		auto polygons = getPolygons(quadtrees);
		auto center = m_bounds->getCenter();

		for (int i = 0; i < polygons.size(); ++i)
		{
			if (auto polygon = polygons[i].lock())
			{
				std::vector<sf::Vector2f> points;
				points.reserve(polygon->getPointCount()*2);
				for (int j = 0; j < polygon->getPointCount(); ++j)
				{
					sf::Vector2f point = polygon->getPoint(j);
					math::extendSegment<float>(center, point, 10000);
					points.push_back(polygon->getPoint(j));
					points.push_back(point);
//					m_vertexArray.append(sf::Vertex(polygon->getPoint(j), sf::Color(0, 0, 0, 100)));
//					m_vertexArray.append(sf::Vertex(point, sf::Color(0, 0, 0, 100)));
				}

				points = math::ConvexHull<float>(points);
				
				m_shadows.push_back(std::unique_ptr<sf::ConvexShape>(new sf::ConvexShape()));
				m_shadows.back()->setPointCount(points.size());
				m_shadows.back()->setFillColor(sf::Color(0, 0, 0, 255));

				for (int j = 0; j < points.size(); ++j)
					m_shadows.back()->setPoint(j, points[j]);
			}
		}
	}
}

std::vector<std::weak_ptr<math::Polygon>> ShadowCaster::getPolygons(const std::vector<std::weak_ptr<Quadtree>>& quadtrees) const
{
	std::vector<std::weak_ptr<math::Polygon>> polygons;

	if (!quadtrees.empty())
	{
		std::vector<unsigned int> indices;
		for (int i = 0; i < quadtrees.size(); ++i)
		{
			if (auto _quadtree = quadtrees[i].lock())
			{
				_quadtree->populateWithPolygons(polygons, indices);
			}
		}

		int index = polygons.size();

		for (int i = polygons.size() - 1; i >= 0; --i)
		{
			if (polygons[i].lock())
			{
				index = i + 1;
				break;
			}
		}

		for (int i = polygons.size() - 1; i >= index; --i)
		{
			if (!polygons[i].lock())
			{
				polygons.pop_back();
				indices.pop_back();
			}
			else
			{
				break;
			}
		}
	}

	return polygons;
}

void ShadowCaster::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_shadows.size(); ++i)
		target.draw(*m_shadows[i], states);
}

