#include "Map.hpp"


Map::Map() :
	m_index(0)
{
	m_quadtree.reset(new Quadtree(sf::Vector2f(0, 0), sf::Vector2f(2048*2, 2048*2), false, 0));
	m_textureHolder.reset(new TextureHolder());
	m_textureHolder->loadTextures("level.mtl");
}

Map::~Map()
{
}

void Map::addPolygon(const math::Polygon& polygon, const std::string& mtl)
{
	m_quadtree->insert(std::shared_ptr<math::Polygon>(new math::Polygon(polygon)), m_index);

	sf::ConvexShape shape;
	shape.setPointCount(polygon.getPointCount());

	for (int i = 0; i < shape.getPointCount(); ++i)
		shape.setPoint(i, polygon.getPoint(i));

	if (mtl != "(null)")
	{
		shape.setTexture(m_textureHolder->getTexture(mtl).lock().get());
	}
	
	m_polygons[m_index] = shape;
	m_index++;
}

void Map::removePolygon(std::shared_ptr<math::Polygon> polygon)
{
//	m_quadtree->remove(polygon);
}

std::shared_ptr<math::Polygon> Map::getPolygon(const sf::Vector2f& position)
{
	return m_quadtree->getPolygon(position);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
		target.draw(it->second, states);
}

void Map::save() const
{
	std::ofstream file;
	file.open("level");
	std::vector<unsigned char> saved;
	m_quadtree->save(file, saved);
}

void Map::load(const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);

	if (file.is_open())
	{
		std::string line;
		std::vector<sf::Vector2f> points;
		while (!file.eof())
		{
			std::getline(file, line);
			if ((line.substr(0, 6) == "usemtl" || line[0] == 'o') && !points.empty()) // new polygon
			{
				math::Polygon polygon;

				if (points.size() > 3)
					points = math::ConvexHull<float>(points);

				for (int i = 0; i < points.size(); ++i)
					polygon.addPoint(points[i]);

				polygon.constructEdges();
				addPolygon(polygon, line.substr(0, 6) == "usemtl" ? line.substr(7, line.size() - 1):"");
				points.clear();
			}
			else if (line[0] == 'v')
			{
				std::string x, y;
				bool _x = true;

				for (int i = 2; i < line.size(); ++i)
				{
					if (line[i] == '.')
					{
						_x = false;
						i += 16;
					}
					else
					{
						if (_x)
							x += line[i];
						else
							y += line[i];
					}
				}

				points.push_back(sf::Vector2f(std::atoi(x.c_str()), std::atoi(y.c_str())));
			}
		}
	}
}

std::weak_ptr<Quadtree> Map::getQuadtree() const
{
	return m_quadtree;
}

