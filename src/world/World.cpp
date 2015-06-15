#include "World.hpp"


World::World(const uint8_t& seed) :
	m_seed(seed)
{
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_rockGenerator.reset(new RockGenerator());
	m_pathGenerator.reset(new PathGenerator());
	m_actorManager.reset(new ActorManager());
	m_npcSpawner.reset(new NPCSpawner());
	m_camera.reset(new Camera());
	m_camera->setSize(1600, 900);
	m_textureHolder->loadTextures("assets/Textures.lst");
	m_sprite.setTexture(*m_textureHolder->getTexture("test").lock());

	m_playerActor.reset(new PlayerActor());
	m_playerActor->setPosition(16000, 16000);
	m_playerActor->setSize(16, 72);
	m_playerActor->setTexture(m_textureHolder->getTexture("char"));
	m_playerActor->assign(m_actorIdTracker->addActor());
	m_camera->trackActor(m_playerActor);

	m_actorManager->addActor(m_playerActor);

	for (int i = 0; i < 0; ++i)
		m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), {(float)(rand()%100), (float)(rand()%100)}, *m_actorManager, *m_actorIdTracker);

	Object object;
	int rocks = 0;
	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(3 + rand()%5, {2, 1}, {std::cos(angle)*(rand()%2000), std::sin(angle)*(rand()%2000)});

		object.setPolygon(polygon);
		addObject(object);
	}

	m_pathGenerator->setTexture(m_textureHolder->getTexture("thing"));
	/*float deviation = M_PI/8;
	m_paths.push_back(m_pathGenerator->generatePath({-128, 0}, {256, 1024}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath({0, 0}, {128, 128}, 4, deviation));
	m_paths.push_back(m_pathGenerator->generatePath({128, 128}, {192, 256}, 4, deviation));
	m_paths.push_back(m_pathGenerator->generatePath({192, 256}, {256, 512}, 8, deviation));
	m_paths.push_back(m_pathGenerator->generatePath({256, 512}, {256, 1024}, 16, deviation));
	*/
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
	int seed = 51;
	seed = ((seed + position.x + m_seed) << 5) - (seed + position.x + m_seed);
	seed = ((seed + position.y + m_seed) << 5) - (seed + position.y + m_seed);
	srand(seed);

	Chunk chunk;
	chunk.setPosition((sf::Vector2f)position);

	chunk.setColor(sf::Color(rand()%255, rand()%255, rand()%255));

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

