#include "ObjectIdTracker.hpp"
#include <iostream>


ObjectIdTracker::ObjectIdTracker()
{
}

ObjectIdTracker::~ObjectIdTracker()
{
}

ObjectId ObjectIdTracker::addObject()
{
	auto id = getFirstAvailableId();
	m_ids.push_back(id);

	return id;
}

void ObjectIdTracker::removeObject(const ObjectId& id)
{
	auto it = std::find(m_ids.begin(), m_ids.end(), id);

	if (it != m_ids.end())
		m_ids.erase(it);
}

ObjectId ObjectIdTracker::getFirstAvailableId() const
{
	if (m_ids.empty())
		return ObjectId{0};

	ObjectId id = *m_ids.begin();
	for (int i = 0; i < m_ids.size(); ++i)
	{
		if (m_ids[i].id - id.id > 1)
		{
			unsigned char _id = id.id + 1;
			return ObjectId{_id};
		}

		id = m_ids[i];
	}

	unsigned char _id = m_ids.back().id + 1;
	return ObjectId{_id};
}

