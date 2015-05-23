#include "actor/NPCSpawner.hpp"


NPCSpawner::NPCSpawner()
{
}

NPCSpawner::~NPCSpawner()
{
}

void NPCSpawner::spawn(const std::string& npcType, const sf::Vector2f& position, ActorManager& actorManager, ActorIdTracker& actorIdTracker)
{
	std::shared_ptr<NPCActor> npcActor;
	npcActor.reset(new NPCActor(npcType, position));

	npcActor->setPosition(position);
	npcActor->assign(actorIdTracker.addActor());
	npcActor->registerLuaStateMachine();

	actorManager.addActor(npcActor);
}

