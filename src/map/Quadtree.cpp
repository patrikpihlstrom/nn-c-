#include "Quadtree.hpp"

Quadtree::Quadtree() :
	m_hasChildren(false),
	m_level(0)
{
}

Quadtree::Quadtree(const sf::Vector2f& position, const sf::Vector2f& dimensions, const bool hasChildren, const unsigned char level) :
	m_position(position),
	m_dimensions(dimensions),
	m_hasChildren(hasChildren),
	m_level(level)
{
	polygon.addPoint((sf::Vector2f)position);
	polygon.addPoint(sf::Vector2f(position.x + dimensions.x, position.y));
	polygon.addPoint(sf::Vector2f(position.x + dimensions.x, position.y + dimensions.y));
	polygon.addPoint(sf::Vector2f(position.x, position.y + dimensions.y));
}

Quadtree::~Quadtree()
{
}

void Quadtree::update()
{
	if (canMergeChildren())
		mergeChildren();
}

void Quadtree::insert(const GameObject& object)
{
	if (!m_hasChildren)
	{
		m_objects.push_back(std::shared_ptr<Object>(new GameObject(object)));
		if (m_level < MAX_LEVEL)
			split();
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, object.getPolygon()))
				m_children[i]->insert(object);
	}
}

void Quadtree::insert(const PlayerEntity& object)
{
	if (!m_hasChildren)
	{
		m_objects.push_back(std::shared_ptr<Object>(new PlayerEntity(object)));
		if (m_level < MAX_LEVEL)
			split();
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, object.getPolygon()))
				m_children[i]->insert(object);
	}
}

void Quadtree::insert(const Light& object)
{
	if (!m_hasChildren)
	{
		m_objects.push_back(std::shared_ptr<Object>(new Light(object)));
		if (m_level < MAX_LEVEL)
			split();
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, object.getPolygon()))
				m_children[i]->insert(object);
	}
}

void Quadtree::insert(const std::shared_ptr<Object> object)
{
	if (!m_hasChildren)
	{
		m_objects.push_back(object);
		if (m_level < MAX_LEVEL)
			split();
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, object->getPolygon()))
				m_children[i]->insert(object);
	}
}

bool Quadtree::remove(const std::shared_ptr<Object> object)
{
	return remove(object->getId());
}

bool Quadtree::remove(const ObjectId& id)
{
	if (!m_children[0])
	{
		auto it = std::find_if(m_objects.begin(), m_objects.end(), compare(id));
		if (it != m_objects.end())
		{
			m_objects.erase(it);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_children[i]->remove(id))
				return true;
		}
	}

	return false;
}

void Quadtree::split()
{
	if (m_hasChildren)
		return;

	m_hasChildren = true;

	m_children[0].reset(new Quadtree(m_position, sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));
	m_children[1].reset(new Quadtree(sf::Vector2f(m_position.x + m_dimensions.x/2, m_position.y), sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));
	m_children[2].reset(new Quadtree(sf::Vector2f(m_position.x, m_position.y + m_dimensions.y/2), sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));
	m_children[3].reset(new Quadtree(sf::Vector2f(m_position.x + m_dimensions.x/2, m_position.y + m_dimensions.y/2), sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < m_objects.size(); ++j)
		{
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, m_objects[j]->getPolygon()))
			{
				m_children[i]->insert(m_objects[j]);
			}
		}
	}

	m_objects.clear();
}

bool Quadtree::canMerge() const
{
	if (auto parent = m_parent.lock())
	{
		for (int i = 0; i < 4; ++i)
		{
			if (!parent->m_children[i]->empty())
				return false;
		}

		return true;
	}

	return false;
}

bool Quadtree::canMergeChildren() const
{
	if (!m_children[0])
		return false;

	for (int i = 0; i < 4; ++i)
	{
		if (!m_children[i]->empty())
			return false;
	}

	return true;
}

bool Quadtree::empty() const
{
	return m_objects.empty();
}

void Quadtree::mergeChildren()
{
	for (int i = 0; i < 4; ++i)
		m_children[i].reset();

	m_hasChildren = false;
}

bool Quadtree::hasChildren() const
{
	return m_hasChildren;
}

void Quadtree::getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const math::Polygon& polygon) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_children[i]->hasChildren())
				m_children[i]->getQuadtrees(quadtrees, polygon);
			else if (math::polygonIntersectsPolygon(polygon, m_children[i]->polygon))
				quadtrees.push_back(m_children[i]);
		}
	}
}

void Quadtree::getObjects(std::vector<std::shared_ptr<Object>>& objects, std::vector<ObjectId>& objectIds) const
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		if (std::find(objectIds.begin(), objectIds.end(), m_objects[i]->getId()) == objectIds.end())
		{
			objectIds.push_back(m_objects[i]->getId());
			objects.push_back(m_objects[i]);
		}
	}
}

std::shared_ptr<Object> Quadtree::getObject(const ObjectId& id) const
{
	if (!m_hasChildren)
	{
		auto it = std::find_if(m_objects.begin(), m_objects.end(), compare(id));

		if (it != m_objects.end())
			return *it;

		return nullptr;
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			auto object = m_children[i]->getObject(id);

			if (object)
				return object;
		}
	}

	return nullptr;
}

void Quadtree::getGameObjects(std::vector<GameObject*>& objects, const math::Polygon& polygon) const
{
	std::vector<ObjectId> ids;
	getGameObjects(objects, polygon, ids);
}

void Quadtree::getLights(std::vector<Light*>& objects, const math::Polygon& polygon) const
{
	std::vector<ObjectId> ids;
	getLights(objects, polygon, ids);
}

void Quadtree::getGameObjects(std::vector<GameObject*>& objects, const math::Polygon& polygon, std::vector<ObjectId>& ids) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_children[i]->getGameObjects(objects, polygon, ids);
		}
	}
	else
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i]->isGameObject())
			{
				if (std::find(ids.begin(), ids.end(), m_objects[i]->getId()) == ids.end())
				{
					ids.push_back(m_objects[i]->getId());
					objects.push_back(static_cast<GameObject*>(m_objects[i].get()));

				}
			}
		}
	}
}

void Quadtree::getLights(std::vector<Light*>& objects, const math::Polygon& polygon, std::vector<ObjectId>& ids) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_children[i]->getLights(objects, polygon, ids);
		}
	}
	else
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i]->isLight())
			{
				if (std::find(ids.begin(), ids.end(), m_objects[i]->getId()) == ids.end())
				{
					if (Light* object = dynamic_cast<Light*>(m_objects[i].get()))
					{
						ids.push_back(m_objects[i]->getId());
						objects.push_back(static_cast<Light*>(m_objects[i].get()));
					}
				}
			}
		}
	}
}

