#include "application/state/GameState.hpp"


GameState::GameState() :
	State()
{
	m_stateType = StateType::Game;

	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_actorManager.reset(new ActorManager());
	m_npcSpawner.reset(new NPCSpawner());

	m_textureHolder->loadTextures("assets/Textures.lst");

	m_playerActor.reset(new PlayerActor());
	m_playerActor->setPosition(0, 0);
	m_playerActor->setSize(16, 72);
	m_playerActor->setTexture(m_textureHolder->getTexture("char"));
	m_playerActor->assign(m_actorIdTracker->addActor());

	m_actorManager->addActor(m_playerActor);
}

GameState::~GameState()
{
}

void GameState::enter()
{
	State::enter();
}

void GameState::update(const float& deltaTime)
{
	m_world.update(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), m_actorManager->getActor({0}).lock()->getPosition(), *m_actorManager, *m_actorIdTracker);

	if (auto camera = m_playerActor->getCamera().lock())
		m_actorManager->update(deltaTime, camera->getBounds<int>());

	State::update(deltaTime);
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (auto camera = m_playerActor->getCamera().lock())
		target.setView(*camera);

	target.draw(m_world, states);
	target.draw(*m_actorManager, states);
}

