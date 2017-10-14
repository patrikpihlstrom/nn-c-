#include "actor/NPCSpawner.hpp"


NPCSpawner::NPCSpawner()
{
}

NPCSpawner::~NPCSpawner()
{
}

void NPCSpawner::spawn(const std::string& npcType, const std::weak_ptr<sf::Texture> texture, const sf::Vector2f& position, ActorManager& actorManager, ActorIdTracker& actorIdTracker)
{
	std::shared_ptr<NPCActor> npcActor;
	npcActor.reset(new NPCActor("test"));
	npcActor->setTexture(texture);
	npcActor->setPosition(position);
	npcActor->setOrigin(position.x, position.y);
	npcActor->assign(actorIdTracker.addActor());
	actorManager.addActor(npcActor);
}

