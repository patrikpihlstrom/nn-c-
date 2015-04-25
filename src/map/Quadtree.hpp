#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "../application/Math.hpp"
#include "../object/GameObject.hpp"
#include "../object/Light.hpp"
#include "../object/Entity.hpp"
#include "../object/PlayerEntity.hpp"


const unsigned char MAX_LEVEL = 0;

class Quadtree
{
public:
	Quadtree();
	Quadtree(const sf::Vector2f& position, const sf::Vector2f& dimensions, const bool hasChildren = false, const unsigned char level = 0);
	~Quadtree();

	void update();

	void insert(const GameObject& object);
	void insert(const PlayerEntity& object);
	void insert(const Light& object);
	void insert(const std::shared_ptr<Object> object);

	bool remove(const std::shared_ptr<Object> object);
	bool remove(const ObjectId& id);

	bool canMerge() const; // Checks itself and its siblings.
	bool canMergeChildren() const;
	bool empty() const;
	bool hasChildren() const;

	math::Polygon polygon;

	void getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const math::Polygon& polygon) const;
	void getObjects(std::vector<std::shared_ptr<Object>>& objects, std::vector<ObjectId>& objectIds) const;

	void getGameObjects(std::vector<GameObject*>& objects, const math::Polygon& polygon) const;
	void getLights(std::vector<Light*>& objects, const math::Polygon& polygon) const;

	std::shared_ptr<Object> getObject(const ObjectId& id) const;

	struct compare : public std::unary_function<ObjectId, bool>
	{
		explicit compare(const ObjectId& id) :
			id(id)
		{
		}

		bool operator() (const GameObject& arg)
		{
			return arg.getId() == id;
		}

		bool operator() (const PlayerEntity& arg)
		{
			return arg.getId() == id;
		}

		bool operator() (const Light& arg)
		{
			return arg.getId() == id;
		}

		bool operator() (const std::shared_ptr<Object> arg)
		{
			return arg->getId() == id;
		}

		ObjectId id;
	};

private:
	std::shared_ptr<Quadtree> m_children[4];
	std::weak_ptr<Quadtree> m_parent;
	std::vector<std::shared_ptr<Object>> m_objects;

	void split();
	void mergeChildren();
	bool m_hasChildren;

	unsigned char m_level;

	sf::Vector2f m_position, m_dimensions;

	void getGameObjects(std::vector<GameObject*>& objects, const math::Polygon& polygon, std::vector<ObjectId>& ids) const;
	void getLights(std::vector<Light*>& objects, const math::Polygon& polygon, std::vector<ObjectId>& ids) const;
};

