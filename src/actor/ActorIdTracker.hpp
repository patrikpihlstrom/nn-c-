#pragma once

#include <vector>
#include <algorithm>

#include "actor/ActorId.hpp"


class ActorIdTracker
{
public:
	ActorIdTracker();
	~ActorIdTracker();

	ActorId addActor();
	void removeActor(const ActorId& id);

private:
	std::vector<ActorId> m_ids;
	ActorId getFirstAvailableId() const;
};

