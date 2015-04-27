#include "Quadtree.hpp"

Quadtree::Quadtree() :
	m_hasChildren(false),
	m_level(0)
{
}

Quadtree::Quadtree(const sf::Rect<int>& boundingBox, const bool hasChildren, const unsigned char level) :
	m_hasChildren(hasChildren),
	m_level(level),
	m_boundingBox(boundingBox)
{
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
			if (m_children[i]->getBoundingBox().intersects(object.getBoundingBox()))
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
			if (m_children[i]->getBoundingBox().intersects(object.getBoundingBox()))
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
			if (m_children[i]->getBoundingBox().intersects(object.getBoundingBox()))
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
			if (m_children[i]->getBoundingBox().intersects(object->getBoundingBox()))
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

	m_children[0].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left, m_boundingBox.top, m_boundingBox.width/2, m_boundingBox.height/2), false, m_level + 1));
	m_children[1].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left + m_boundingBox.width/2, m_boundingBox.top, m_boundingBox.width/2, m_boundingBox.height/2), false, m_level + 1));
	m_children[2].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left, m_boundingBox.top + m_boundingBox.height/2, m_boundingBox.width/2, m_boundingBox.height/2), false, m_level + 1));
	m_children[3].reset(new Quadtree(sf::Rect<int>(m_boundingBox.left + m_boundingBox.width/2, m_boundingBox.top + m_boundingBox.height/2, m_boundingBox.width/2, m_boundingBox.height/2), false, m_level + 1));

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < m_objects.size(); ++j)
		{
			if (m_children[i]->getBoundingBox().intersects(m_objects[i]->getBoundingBox()))
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

	m_hasChildren = false;
}

bool Quadtree::hasChildren() const
{
	return m_hasChildren;
}

void Quadtree::getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const sf::Rect<int>& boundingBox) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_children[i]->hasChildren())
				m_children[i]->getQuadtrees(quadtrees, boundingBox);
			else if (m_children[i]->getBoundingBox().intersects(boundingBox))
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

void Quadtree::getGameObjects(std::vector<GameObject*>& objects, const sf::Rect<int>& boundingBox) const
{
	std::vector<ObjectId> ids;
	getGameObjects(objects, boundingBox, ids);
}

void Quadtree::getLights(std::vector<Light*>& objects, const sf::Rect<int>& boundingBox) const
{
	std::vector<ObjectId> ids;
	getLights(objects, boundingBox, ids);
}

void Quadtree::getGameObjects(std::vector<GameObject*>& objects, const sf::Rect<int>& boundingBox, std::vector<ObjectId>& ids) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (boundingBox.intersects(m_children[i]->getBoundingBox()))
				m_children[i]->getGameObjects(objects, boundingBox, ids);
		}
	}
	else
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i]->isGameObject())
			{
				if (ids.empty() || std::find(ids.begin(), ids.end(), m_objects[i]->getId()) == ids.end())
				{
					if (boundingBox.intersects(m_objects[i]->getBoundingBox()))
					{
						ids.push_back(m_objects[i]->getId());
						objects.push_back(static_cast<GameObject*>(m_objects[i].get()));
					}

				}
			}
		}
	}
}

void Quadtree::getLights(std::vector<Light*>& objects, const sf::Rect<int>& boundingBox, std::vector<ObjectId>& ids) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (boundingBox.intersects(m_children[i]->getBoundingBox()))
				m_children[i]->getLights(objects, boundingBox, ids);
		}
	}
	else
	{
		for (int i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i]->isLight())
			{
				if (ids.empty() || std::find(ids.begin(), ids.end(), m_objects[i]->getId()) == ids.end())
				{
					if (boundingBox.intersects(m_objects[i]->getBoundingBox()))
					{
						ids.push_back(m_objects[i]->getId());
						objects.push_back(static_cast<Light*>(m_objects[i].get()));
					}
				}
			}
		}
	}
}

sf::Rect<int> Quadtree::getBoundingBox() const
{
	return m_boundingBox;
}

