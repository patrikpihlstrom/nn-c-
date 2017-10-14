#pragma once

#include "actor/Actor.hpp"


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

private:
	void control();

	std::string m_npcType;

	std::weak_ptr<Actor> m_playerActor;
};

