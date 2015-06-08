#pragma once

#include "actor/Actor.hpp"
#include "ai/StateMachine.hpp"


class NPCActor : public Actor, public std::enable_shared_from_this<NPCActor>
{
public:
	NPCActor();
	NPCActor(const std::string& npcType);
	NPCActor(const std::string& npcType, const sf::Vector2f& position);
	~NPCActor();

	uint8_t getHealth() const;
	bool isPlayer() const;
	bool isNPC() const;

	void update(const float& deltaTime);

	void registerLuaStateMachine();
	std::weak_ptr<StateMachine<NPCActor>> getStateMachine();

	void changeState(const luabind::object& state);

private:
	void control();

	std::shared_ptr<StateMachine<NPCActor>> m_stateMachine;
	lua_State* m_luaState;

	std::string m_npcType;
};

