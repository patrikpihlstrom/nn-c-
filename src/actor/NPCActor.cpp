#include "actor/NPCActor.hpp"


NPCActor::NPCActor() : 
	Actor()
{
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);

	if (int error = luaL_dofile(m_luaState, "src/actor/test.out") != 0)
		std::cout << "[LUA]: " << error << std::endl;

	lua_getglobal(m_luaState, "velocity");
	if (lua_istable(m_luaState, 1))
	{
		lua_pushnil(m_luaState);

		while (lua_next(m_luaState, -2) != 0)
		{
			if (lua_isnumber(m_luaState, -1))
			{
				std::string string = lua_tostring(m_luaState, -2);
				std::cout << string << std::endl;
				if (string == "x")
				{
					m_velocity.x = lua_tonumber(m_luaState, -1);
					std::cout << "x: " << getPosition().x << std::endl;
				}
				else
				{
					m_velocity.y = lua_tonumber(m_luaState, -1);
					std::cout << "y: " << getPosition().y << std::endl;
				}
			}

			lua_pop(m_luaState, 1);
		}
	}

	std::cout << std::endl;
}

NPCActor::~NPCActor()
{
	std::cout << "Closing Lua state for actor: " << m_id.id << std::endl;
	lua_close(m_luaState);
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

