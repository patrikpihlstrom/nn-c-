#include "actor/ActorIdTracker.hpp"


ActorIdTracker::ActorIdTracker()
{
}

ActorIdTracker::~ActorIdTracker()
{
}

ActorId ActorIdTracker::addActor()
{
	auto id = getFirstAvailableId();
	m_ids.push_back(id);

	return id;
}

void ActorIdTracker::removeActor(const ActorId& id)
{
	auto it = std::find(m_ids.begin(), m_ids.end(), id);

	if (it != m_ids.end())
		m_ids.erase(it);
}

ActorId ActorIdTracker::getFirstAvailableId() const
{
	if (m_ids.empty())
		return ActorId{0};

	ActorId id = *m_ids.begin();
	for (int i = 0; i < m_ids.size(); ++i)
	{
		if (m_ids[i].id - id.id > 1)
		{
			unsigned long _id = id.id + 1;
			return ActorId{_id};
		}

		id = m_ids[i];
	}

	unsigned long _id = m_ids.back().id + 1;
	return ActorId{_id};
}

