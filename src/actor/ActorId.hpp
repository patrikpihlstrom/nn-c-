#pragma once

#include <cstdint>


struct ActorId
{
	uint64_t id;

	bool operator== (const ActorId& compare) const
	{
		return id == compare.id;
	}

	bool operator< (const ActorId& compare) const
	{
		return id == compare.id;
	}
};

