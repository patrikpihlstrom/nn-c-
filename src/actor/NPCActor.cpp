#include "actor/NPCActor.hpp"


NPCActor::NPCActor() : 
	Actor()
{
}

NPCActor::~NPCActor()
{
}

bool NPCActor::isPlayer() const
{
	return false;
}

bool NPCActor::isNPC() const
{
	return true;
}

