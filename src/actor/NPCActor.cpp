#include "actor/NPCActor.hpp"


NPCActor::NPCActor() : 
	Actor()
{
}

NPCActor::NPCActor(const std::string& npcType, const sf::Vector2f& position) :
	Actor(),
	m_npcType(npcType)
{
	setPositionMaster(position.x, position.y);
}

NPCActor::~NPCActor()
{
}

bool NPCActor::isPlayer() const
{
	return false;
}

bool NPCActor::isNPC() const
{
	return true;
}

void NPCActor::update(const float& deltaTime)
{
	if (m_stateMachine)
		m_stateMachine->update();

	control();
	updatePosition(deltaTime);
}

void NPCActor::registerLuaStateMachine()
{
	m_stateMachine.reset(new StateMachine<NPCActor>(shared_from_this()));
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);
	luabind::open(m_luaState);

	luabind::module(m_luaState)
	[
		luabind::class_<Actor>("Actor")
		.def("getHealth", &Actor::getHealth)
		.def("setHealth", &Actor::setHealth)
		.def("damage", &Actor::damage)
		.def("isNPC", &Actor::isNPC)
		.def("isPlayer", &Actor::isPlayer)
		.def("getId", &Actor::getId)
		.def("getBounds", &Actor::getBounds)
		.def("setBounds", &Actor::setBounds)
		.def("getPolygon", &Actor::getPolygon)
		.def("setPolygon", &Actor::setPolygon)
		.def("getShape", &Actor::getShape)
		.def("setShape", &Actor::setShape)
		.def("setVelocity", &Actor::setVelocity)
		.def("setPosition", &Actor::setPositionMaster)
		.def("getPositionX", &Actor::getPositionX)
		.def("getPositionY", &Actor::getPositionY)
		.def("getVelocityX", &Actor::getVelocityX)
		.def("getVelocityY", &Actor::getVelocityY)
		.def("move", &Actor::move)
	];

	luabind::module(m_luaState)
	[
		luabind::class_<NPCActor, luabind::bases<Actor>>("NPCActor")
		.def("changeState", &NPCActor::changeState)
	];

	luabind::module(m_luaState)
	[
		luabind::class_<StateMachine<NPCActor>>("StateMachine")
		.def("getCurrentState", &StateMachine<NPCActor>::getCurrentState)
		.def("setCurrentState", &StateMachine<NPCActor>::setCurrentState)
	];

	if (int error = luaL_dofile(m_luaState, std::string("assets/npc/" + m_npcType + "/behavior.lua").c_str()) != 0)
		std::cout << "ERROR[" << error << "] Problem with lua script file: " << "assets/npc/" << m_npcType << "/behavior.lua" << std::endl;

	luabind::object states = luabind::globals(m_luaState);

	if (luabind::type(states) == LUA_TTABLE)
		m_stateMachine->setCurrentState(states["State_Init"]);
}

void NPCActor::control()
{
	/*if (math::distance<float>(m_target, getPosition()) <= 8)
	{
		m_target = {(float)(-500 + rand()%1000), (float)(-500 + rand()%1000)};

		float angle = math::angle<float>(m_target, getPosition());
		m_velocity.x = std::cos(angle)*600;
		m_velocity.y = std::sin(angle)*600;
	}*/
}

void NPCActor::changeState(const luabind::object& state)
{
	if (m_stateMachine)
		m_stateMachine->changeState(state);
}

