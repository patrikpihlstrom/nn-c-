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
	m_playerActor->setTexture(m_textureHolder->getTexture("char"));
	m_playerActor->setSize(16, 72);
	m_playerActor->assign(m_actorIdTracker->addActor());

	m_actorManager->addActor(m_playerActor);

	m_world = World(time(NULL));
}

GameState::~GameState()
{
}

void GameState::enter(sf::RenderWindow& window)
{
	if (auto camera = m_playerActor->getCamera().lock())
		window.setView(*camera);

	State::enter(window);
}

void GameState::update(const float& deltaTime, const sf::RenderWindow& window)
{
	m_world.update(deltaTime, (sf::Vector2i)m_playerActor->getPosition());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), m_actorManager->getActor({0}).lock()->getPosition(), *m_actorManager, *m_actorIdTracker);
	else if (sf::Joystick::isConnected(0))
	{
		if (sf::Joystick::isButtonPressed(0, 1))
			m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), m_actorManager->getActor({0}).lock()->getPosition(), *m_actorManager, *m_actorIdTracker);
	}

	if (auto camera = m_playerActor->getCamera().lock())
		m_actorManager->update(deltaTime, camera->getBounds<int>());

	State::update(deltaTime, window);
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (auto camera = m_playerActor->getCamera().lock())
		target.setView(*camera);

	target.draw(m_world, states);
	target.draw(*m_actorManager, states);
}

void GameState::exit()
{
	State::exit();
}

sf::Vector2f GameState::getView() const
{
	return m_playerActor->getCamera().lock()->getCenter();
}

