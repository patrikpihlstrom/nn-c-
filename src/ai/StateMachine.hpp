#pragma once

#include <memory>

#include "application/lua.hpp"


template<class Actor>
class StateMachine : public std::enable_shared_from_this<StateMachine<Actor>>
{
public:
	StateMachine(){}
	StateMachine(std::weak_ptr<Actor> actor) : m_actor(actor){}
	~StateMachine(){}

	void setActor(std::weak_ptr<Actor> actor)
	{
		if (actor.lock())
			m_actor = actor;
	}

	void setCurrentState(const luabind::object& state)
	{
		if (state.is_valid())
			m_currentState = state;

		if (auto actor = m_actor.lock())
			m_currentState["Enter"](actor.get());
	}

	void changeState(const luabind::object& state)
	{
		if (!state.is_valid())
			return;

		if (auto actor = m_actor.lock())
		{
			if (m_currentState.is_valid())
				m_currentState["Exit"](actor.get());

			m_currentState = state;
			m_currentState["Enter"](actor.get());
		}
	}

	void update()
	{
		if (auto actor = m_actor.lock())
		{
			if (m_currentState.is_valid())
			{
				m_currentState["Execute"](actor.get());
			}
		}
	}

	luabind::object getCurrentState() const
	{
		return m_currentState;
	}

private:
	std::weak_ptr<Actor> m_actor;
	luabind::object m_currentState;
};

