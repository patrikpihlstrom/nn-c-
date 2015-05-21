#include "Quadtree.hpp"

Quadtree::Quadtree() :
	m_level(0)
{
}

Quadtree::Quadtree(const sf::Rect<int>& boundingBox, const unsigned char level) :
	m_level(level),
	m_boundingBox(boundingBox)
{
}

Quadtree::~Quadtree()
{
}

void Quadtree::update()
{
	if (!m_children[0])
	{
		if (auto parent = m_parent.lock())
			parent->update();
		else
			std::cout << m_level << std::endl;
	}
	else if (canMergeChildren())
	{
		mergeChildren();

		if (auto parent = m_parent.lock())
			parent->update();
		else
			std::cout << m_level << std::endl;
	}
}

void Quadtree::insert(const std::shared_ptr<Object> object)
{
	if (object->getBoundingBox().intersects(getBoundingBox()))
	{
		if (!m_children[0])
		{
			m_objects.push_back(object);
			if (m_level < MAX_LEVEL)
				split();
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				if (m_children[i]->getBoundingBox().intersects(object->getBoundingBox()))
				{
					m_children[i]->insert(object);
				}
			}
		}
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
	m_children[0].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left, m_boundingBox.top, m_boundingBox.width/2, m_boundingBox.height/2), m_level + 1));
	m_children[1].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left + m_boundingBox.width/2, m_boundingBox.top, m_boundingBox.width/2, m_boundingBox.height/2), m_level + 1));
	m_children[2].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left, m_boundingBox.top + m_boundingBox.height/2, m_boundingBox.width/2, m_boundingBox.height/2), m_level + 1));
	m_children[3].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left + m_boundingBox.width/2, m_boundingBox.top + m_boundingBox.height/2, m_boundingBox.width/2, m_boundingBox.height/2), m_level + 1));

	for (int i = 0; i < 4; ++i)
	{
		m_children[i]->m_parent = shared_from_this();
		for (int j = 0; j < m_objects.size(); ++j)
		{
			m_children[i]->insert(m_objects[j]);
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
}

void Quadtree::getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const sf::Rect<int>& boundingBox) const
{
	if (m_children[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_children[i]->getBoundingBox().intersects(boundingBox))
			{
				if (m_children[i]->m_children[0])
				{
					m_children[i]->getQuadtrees(quadtrees, boundingBox);
				}
				else
					quadtrees.push_back(m_children[i]);
			}
		}
	}
}

void Quadtree::getObjects(std::vector<std::weak_ptr<Object>>& objects, std::vector<ObjectId>& objectIds) const
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

std::vector<std::weak_ptr<Object>> Quadtree::getObjects(const sf::Rect<int>& boundingBox) const
{
	std::vector<std::weak_ptr<Quadtree>> quadtrees;
	getQuadtrees(quadtrees, boundingBox);

	std::vector<ObjectId> objectIds;
	std::vector<std::weak_ptr<Object>> objects;

	for (int i = 0; i < quadtrees.size(); ++i)
	{
		if (auto quadtree = quadtrees[i].lock())
			quadtree->getObjects(objects, objectIds);
	}

	return objects;
}

std::shared_ptr<Object> Quadtree::getObject(const ObjectId& id) const
{
	if (!m_children[0])
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

sf::Rect<int> Quadtree::getBoundingBox() const
{
	return m_boundingBox;
}

void Quadtree::draw(const sf::Rect<int>& bounds, sf::RenderTarget& target, sf::RenderStates states) const
{
	std::vector<std::weak_ptr<Quadtree>> quadtrees;
	getQuadtrees(quadtrees, bounds);

	for (int i = 0; i < quadtrees.size(); ++i)
	{
		if (auto quadtree = quadtrees[i].lock())
		{
			quadtree->draw(target, states);
		}
	}
}

void Quadtree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/*sf::RectangleShape rect;
	rect.setPosition(m_boundingBox.left, m_boundingBox.top);
	rect.setSize(sf::Vector2f(m_boundingBox.width, m_boundingBox.height));
	rect.setFillColor(sf::Color(255, 0, 0, 25));
	rect.setOutlineColor(sf::Color(255, 0, 0));
	rect.setOutlineThickness(1.f);
	target.draw(rect, states);
	*/

	if (!m_children[0])
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			m_objects[i]->draw(target, states);
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			m_children[i]->draw(target, states);
	}
}

