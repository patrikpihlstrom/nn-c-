#pragma once

#include <vector>
#include <algorithm>

#include "ObjectId.hpp"


class ObjectIdTracker
{
public:
	ObjectIdTracker();
	~ObjectIdTracker();

	ObjectId addObject();
	void removeObject(const ObjectId& id);

private:
	std::vector<ObjectId> m_ids;
	ObjectId getFirstAvailableId() const;
};

