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
	npcActor.reset(new NPCActor("test"));
	npcActor->setPosition(position);
	npcActor->registerLuaStateMachine();
	npcActor->assign(actorIdTracker.addActor());
	actorManager.addActor(npcActor);
}

