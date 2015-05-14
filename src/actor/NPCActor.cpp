#include "actor/NPCActor.hpp"


NPCActor::NPCActor() : 
	Actor()
{
	m_target = {(float)(-500 + rand()%1000), (float)(-500 + rand()%1000)};
	float angle = math::angle<float>(m_target, getPosition());
	m_velocity.x = std::cos(angle)*600;
	m_velocity.y = std::sin(angle)*600;
	
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);

	if (int error = luaL_dofile(m_luaState, "src/actor/test.lua") != 0)
		std::cout << "[LUA]: " << error << std::endl;

	lua_close(m_luaState);
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

