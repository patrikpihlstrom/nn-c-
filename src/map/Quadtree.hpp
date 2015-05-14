#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "application/Math.hpp"
#include "object/Object.hpp"


const unsigned char MAX_LEVEL = 3;

class Quadtree : public std::enable_shared_from_this<Quadtree>
{
public:
	Quadtree();
	Quadtree(const sf::Rect<int>& boundingBox, const unsigned char level = 0);
	~Quadtree();

	void update();

	void insert(const std::shared_ptr<Object> object);

	bool remove(const std::shared_ptr<Object> object);
	bool remove(const ObjectId& id);

	bool canMerge() const; // Checks itself and its siblings.
	bool canMergeChildren() const;
	bool empty() const;

	void getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const sf::Rect<int>& boundingBox) const;
	void getObjects(std::vector<std::shared_ptr<Object>>& objects, std::vector<ObjectId>& objectIds) const;

	std::shared_ptr<Object> getObject(const ObjectId& id) const;

	struct compare : public std::unary_function<ObjectId, bool>
	{
		explicit compare(const ObjectId& id) :
			id(id)
		{
		}

		bool operator() (const Object& arg)
		{
			return arg.getId() == id;
		}

		bool operator() (const std::shared_ptr<Object> arg)
		{
			return arg->getId() == id;
		}

		ObjectId id;
	};

	sf::Rect<int> getBoundingBox() const;

	void draw(const sf::Rect<int>& bounds, sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::shared_ptr<Quadtree> m_children[4];
	std::weak_ptr<Quadtree> m_parent;
	std::vector<std::shared_ptr<Object>> m_objects;

	void split();
	void mergeChildren();

	unsigned char m_level;

	sf::Rect<int> m_boundingBox;

	void draw(std::vector<ObjectId>& ids, sf::RenderTarget& target, sf::RenderStates states) const;
};

