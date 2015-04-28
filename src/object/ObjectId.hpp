#pragma once


struct ObjectId
{
	unsigned long id;

	bool operator== (const ObjectId& compare) const
	{
		return id == compare.id;
	}

	bool operator< (const ObjectId& compare) const
	{
		return id < compare.id;
	}
};

