#pragma once

#include <memory>

#include "object/Light.hpp"


class LightObstructor
{
public:
	LightObstructor();
	~LightObstructor();

	void addLight(std::weak_ptr<Light> light);
	void removeLight(const ObjectId& id);
	void clearLights();

	// Remove lights not affected by object
	void updateLightsVector();
	void notifyLights();

	void setPolygon(const math::Polygon& polygon);
	math::Polygon getPolygon() const;

private:
	std::vector<std::weak_ptr<Light>> m_lights;

	math::Polygon m_polygon;
};

