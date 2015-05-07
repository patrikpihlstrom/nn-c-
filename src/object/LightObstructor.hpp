#pragma once

#include <memory>

#include "object/Light.hpp"


class LightObstructor
{
public:
	LightObstructor();
	~LightObstructor();

	bool hasLight(const ObjectId& id) const;

	void addLight(std::weak_ptr<Light> light);
	void removeLight(const ObjectId& id);
	void clearLights();

	// Remove lights not affected by object
	void updateLightsVector();
	void notifyLights();

	sf::Rect<int> getBoundingBox() const;
	void setBoundingBox(const sf::Rect<int>& boundingBox);

private:
	std::vector<std::weak_ptr<Light>> m_lights;

	sf::Rect<int> m_boundingBox;

};

