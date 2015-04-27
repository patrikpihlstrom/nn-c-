#pragma once


struct ObjectId
{
	unsigned short id;

	bool operator== (const ObjectId& compare) const
	{
		return id == compare.id;
	}

	bool operator< (const ObjectId& compare) const
	{
		return id < compare.id;
	}
};

