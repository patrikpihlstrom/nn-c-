#include "object/LightObstructor.hpp"


LightObstructor::LightObstructor()
{
}

LightObstructor::~LightObstructor()
{
}

bool LightObstructor::hasLight(const ObjectId& id) const
{
	for (auto it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		if (auto light = it->lock())
		{
			if (light->getId() == id)
				return true;
		}
	}

	return false;
}

void LightObstructor::addLight(std::weak_ptr<Light> light)
{
	if (auto _light = light.lock())
	{
		if (!hasLight(_light->getId()))
			m_lights.push_back(light);
	}
}

void LightObstructor::removeLight(const ObjectId& id)
{
	for (auto it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		if (auto light = it->lock())
		{
			if (light->getId() == id)
			{
				m_lights.erase(it);
				return;
			}
		}
	}
}

void LightObstructor::clearLights()
{
	m_lights.clear();
}

void LightObstructor::updateLightsVector()
{
	for (auto it = m_lights.begin(); it != m_lights.end();)
	{
		if (auto light = it->lock())
		{
			if (!light->getBoundingBox().intersects(m_boundingBox))
				it = m_lights.erase(it);
			else
				++it;
		}
	}
}

void LightObstructor::notifyLights()
{
	for (auto it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		if (auto light = it->lock())
		{
			light->requestUpdate();
		}
	}
}

sf::Rect<int> LightObstructor::getBoundingBox() const
{
	return m_boundingBox;
}

void LightObstructor::setBoundingBox(const sf::Rect<int>& boundingBox)
{
	m_boundingBox = boundingBox;
}

