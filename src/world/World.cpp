#include "World.hpp"


World::World(const long& seed) :
	m_seed(seed)
{
	std::string _seed = std::to_string(m_seed);
	_seed = _seed.substr(_seed.size() - 3, _seed.size());
	m_seed = std::atoi(_seed.c_str());

	std::cout << "SEED: " << m_seed << std::endl;
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_rockGenerator.reset(new RockGenerator());
	m_pathGenerator.reset(new PathGenerator());
	m_actorManager.reset(new ActorManager());
	m_npcSpawner.reset(new NPCSpawner());
	m_noise.reset(new Noise());
	m_camera.reset(new Camera());

	m_noise->setSeed(m_seed);
	m_noise->setPersistence(1);
	m_noise->setFrequency(.01);
	m_noise->setAmplitude(1);
	m_noise->setOctaves(1);

	m_camera->setSize(1600, 900);

	m_textureHolder->loadTextures("assets/Textures.lst");

	m_playerActor.reset(new PlayerActor());
	m_playerActor->setPosition(16000, 16000);
	m_playerActor->setSize(16, 72);
	m_playerActor->setTexture(m_textureHolder->getTexture("char"));
	m_playerActor->assign(m_actorIdTracker->addActor());
	m_camera->trackActor(m_playerActor);

	m_actorManager->addActor(m_playerActor);
}

World::~World()
{
}

void World::update(const float& deltaTime, const sf::RenderWindow& window)
{
	float dt = deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), m_actorManager->getActor({0}).lock()->getPosition(), *m_actorManager, *m_actorIdTracker);

	if (!m_zooming)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			m_camera->zoom(1.1f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			m_camera->zoom(.9f);
	}
	m_zooming = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));

	m_camera->update();
	m_actorManager->update(dt, sf::Rect<int>(m_camera->getCenter().x - m_camera->getSize().x, m_camera->getCenter().y - m_camera->getSize().y, m_camera->getSize().x*2, m_camera->getSize().y*2));

	addChunks(m_playerActor->getPosition());
	removeChunks(m_playerActor->getPosition());
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(m_sprite, states);

	for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
		it->second->draw(target, states);

	m_quadtree->draw(m_camera->getBounds<int>(), target, states);
	m_actorManager->draw(target, states);
}

std::weak_ptr<Quadtree> World::getQuadtree() const
{
	return m_quadtree;
}

std::weak_ptr<ActorManager> World::getActorManager() const
{
	return m_actorManager;
}

Camera World::getCamera() const
{
	return *m_camera;
}

void World::addObject(const Object& object)
{
	std::shared_ptr<Object >newObject(new Object(object));
	newObject->assign(m_objectIdTracker->addObject());
	m_quadtree->insert(newObject);
}

void World::addChunks(const sf::Vector2f& position)
{
	for (int x = position.x/CHUNK_SIZE - m_camera->getSize().x/2/CHUNK_SIZE - 1, y = position.y/CHUNK_SIZE - m_camera->getSize().y/2/CHUNK_SIZE - 1; x < position.x/CHUNK_SIZE + m_camera->getSize().x/2/CHUNK_SIZE + 1; ++x)
	{
		for (y = position.y/CHUNK_SIZE - m_camera->getSize().y/2/CHUNK_SIZE - 1; y < position.y/CHUNK_SIZE + m_camera->getSize().y/2/CHUNK_SIZE + 1; ++y)
		{
			auto chunk = m_chunks.find({x, y});

			if (chunk == m_chunks.end())
			{
				addChunk({x*CHUNK_SIZE, y*CHUNK_SIZE});
			}
		}
	}
}

void World::addChunk(const sf::Vector2i& position)
{
	Chunk chunk;
	chunk.setPosition((sf::Vector2f)position);
	auto noise = m_noise->getHeight(position.x/CHUNK_SIZE, position.y/CHUNK_SIZE);
	chunk.build(std::abs(noise));
	m_chunks.insert(std::pair<sf::Vector2i, std::shared_ptr<Chunk>>(position, std::shared_ptr<Chunk>(new Chunk(chunk))));
}

void World::removeChunks(const sf::Vector2f& position)
{
	auto it = m_chunks.begin();
	while (it != m_chunks.end())
	{
		if (it->second->getPosition().x + CHUNK_SIZE < m_camera->getCenter().x - m_camera->getSize().x/2 || it->second->getPosition().x > m_camera->getCenter().x + m_camera->getSize().x/2 || it->second->getPosition().y + CHUNK_SIZE < m_camera->getCenter().y - m_camera->getSize().y/2 || it->second->getPosition().y > m_camera->getCenter().y + m_camera->getSize().y/2)
			it = m_chunks.erase(it);
		else
			++it;
	}
}

