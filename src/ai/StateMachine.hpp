#pragma once

#include <memory>

#include "application/lua.hpp"

#include "actor/Actor.hpp"


class StateMachine
{
public:
	StateMachine();
	StateMachine(std::weak_ptr<Actor> actor);
	~StateMachine();

private:
	std::weak_ptr<Actor> m_actor;
};

