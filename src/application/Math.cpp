#include "Math.hpp"
#include <iostream>


namespace math
{
	std::vector<sf::Vector2f> cutTri(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d, const sf::Vector2f& e)
	{
		std::vector<sf::Vector2f> points;
		math::Polygon polygon;
		polygon.addPoint(a);
		polygon.addPoint(b);
		polygon.addPoint(c);
		polygon.constructEdges();

		bool containsD = polygon.containsPoint(d);
		bool containsE = polygon.containsPoint(e);

		if (containsD && !containsE)
		{
			sf::Vector2f point;

			for (int i = 0; i < polygon.getEdgeCount(); ++i)
			{
				auto edge = polygon.getEdgeSegment<float>(i);
				if (lineIntersectsLine(edge.a, edge.b, d, e, point))
				{
					points.push_back(a);
					points.push_back(point);
					points.push_back(d);
				}
			}
		}
		else if (containsE && !containsD)
		{
			sf::Vector2f point;

			for (int i = 0; i < polygon.getEdgeCount(); ++i)
			{
				auto edge = polygon.getEdgeSegment<float>(i);
				if (lineIntersectsLine(edge.a, edge.b, d, e, point))
				{
					points.push_back(a);
					points.push_back(point);
					points.push_back(d);
				}
			}
		}
		else if (containsD && containsE)
		{
			
		}

		return points;
	}

	sf::Vector2f getLineIntersection(const sf::Vector2f a, const sf::Vector2f b, const sf::Vector2f c, const sf::Vector2f d, bool& interrupted)
	{
		sf::Vector2f intersection;

		if (lineIntersectsLine(a, b, c, d, intersection))
		{
			interrupted = true;
			return intersection;
		}
		else
			return b;
	}

	bool getLinesIntersection(const sf::Vector2f a, const sf::Vector2f b, const sf::Vector2f c, const sf::Vector2f d, sf::Vector2f& i)
	{
		float s1_x, s1_y, s2_x, s2_y;
		s1_x = b.x - a.x;
		s1_y = b.y - a.y;
		s2_x = d.x - c.x;
		s2_y = d.y - c.y;

		float s, t;
		s = (-s1_y * (a.x - c.x) + s1_x * (a.y - c.y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = ( s2_x * (a.y - c.y) - s2_y * (a.x - c.x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			i.x = a.x + (t * s1_x);
			i.y = a.y + (t * s1_y);
			return true;
		}

		return false;
	}

	sf::Vector2f getLineRectIntersection(const sf::Vector2f a, const sf::Vector2f b, const sf::Rect<float> rect, bool& interrupted)
	{
		float len = distance(a, b);
		sf::Vector2f point = b;
		sf::Vector2f potential = b;

		if (getLinesIntersection(a, b, {rect.left, rect.top}, {rect.left, rect.top + rect.height}, potential))
		{
			point = potential;
			len = distance(a, point);
			interrupted = true;
		}

		if (getLinesIntersection(a, b, {rect.left + rect.width, rect.top}, {rect.left + rect.width, rect.top + rect.height}, potential))
		{
			if (len > distance(a, potential))
			{
				point = potential;
				len = distance(a, point);
			}

			interrupted = true;
		}

		if (getLinesIntersection(a, b, {rect.left, rect.top}, {rect.left + rect.width, rect.top}, potential))
		{
			if (len > distance(a, potential))
			{
				point = potential;
				len = distance(a, point);
			}

			interrupted = true;
		}

		if (getLinesIntersection(a, b, {rect.left, rect.top + rect.height}, {rect.left + rect.width, rect.top + rect.height}, potential))
		{
			if (len > distance(a, potential))
			{
				point = potential;
			}

			interrupted = true;
		}

		return point;
	}

	bool lineIntersectsPolygon(const sf::Vector2f& a, const sf::Vector2f& b, const Polygon& polygon)
	{
		for (int i = 0; i < polygon.getEdgeCount(); ++i)
		{
			auto segment = polygon.getEdgeSegment<float>(i);

			if (lineIntersectsLine(a, b, segment.a, segment.b))
				return true;
		}

		return false;
	}

	bool lineIntersectsPolygon(const sf::Vector2f& a, const sf::Vector2f& b, sf::Vector2f& c, sf::Vector2f& d, const Polygon& polygon)
	{
		bool result = false;

		for (int i = 0; i < polygon.getEdgeCount(); ++i)
		{
			auto segment = polygon.getEdgeSegment<float>(i);

			if (result)
				lineIntersectsLine(a, b, segment.a, segment.b, d);
			else if (lineIntersectsLine(a, b, segment.a, segment.b, c))
				result = true;
		}

		return result;
	}

	bool rectIntersectsPolygon(const sf::Rect<float> rect, const Polygon& polygon)
	{
		sf::Vector2f rectLines[4] = {{rect.left, rect.top}, {rect.left + rect.width, rect.top}, {rect.left + rect.width, rect.top + rect.height}, {rect.left, rect.top + rect.height}};
		
		bool interrupted = false;

		for (int i = 0; i < polygon.getEdgeCount(); ++i)
		{
			auto segment = polygon.getEdgeSegment<float>(i);
			auto p = getLineRectIntersection(segment.a, segment.b, rect, interrupted);

			if (interrupted)
			{
				return true;
			}
		}

		return false;
	}

	bool pointInPolygon(const sf::Vector2f& point, const Polygon& polygon)
	{
		auto bounds = polygon.getBounds();
		if (bounds.contains(point.x, point.y))
		{
			sf::Vector2f _point{point.x, (float)bounds.top - 10};

			int intersections = -1;
			for (int i = 0; i < polygon.getPointCount() - 1; ++i)
			{
				if (lineIntersectsLine(point, _point, polygon.getPoint(i), polygon.getPoint(i + 1)))
					intersections++;
			}

			return (intersections%2 == 0);
		}
		
		return false;
	}

	void normalize(sf::Vector2f& vector)
	{
		float magnitude = math::magnitude<float>(vector);
		vector.x /= magnitude;
		vector.y /= magnitude;
	}

	void projectPolygon(const sf::Vector2f& axis, const Polygon& polygon, float& min, float& max)
	{
		float dotProduct = math::dotProduct<float>(axis, polygon.getPoint(0));
		min = dotProduct;
		max = dotProduct;

		for (int i = 0; i < polygon.getPointCount(); ++i)
		{
			dotProduct = math::dotProduct<float>(axis, polygon.getPoint(i));

			if (dotProduct < min)
				min = dotProduct;
			else if (dotProduct > max)
				max = dotProduct;
		}
	}

	float intervalDistance(const float& minA, const float& maxA, const float& minB, const float& maxB)
	{
		if (minA < minB)
			return minB - maxA;
		else
			return minA - maxB;
	}

	bool polygonIntersectsPolygon(const Polygon& a, const Polygon& b)
	{
		float minIntervalDistance = std::numeric_limits<float>::infinity();
		sf::Vector2f translationAxis, edge, centerA = a.getCenter(), centerB = b.getCenter();

		for (int edgeIndex = 0; edgeIndex < a.getEdgeCount() + b.getEdgeCount(); edgeIndex++)
		{
			if (edgeIndex < a.getEdgeCount())
				edge = a.getEdge(edgeIndex);
			else
				edge = b.getEdge(edgeIndex - a.getEdgeCount());

			sf::Vector2f axis = sf::Vector2f(-edge.y, edge.x);
			math::normalize(axis);

			float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
			math::projectPolygon(axis, a, minA, maxA);
			math::projectPolygon(axis, b, minB, maxB);

			if (math::intervalDistance(minA, maxA, minB, maxB) >= 0)
				return false;
		}

		return true;
	}

	Intersection SAT(const Polygon& a, const Polygon& b, const sf::Vector2f& velocity, const bool MTV)
	{
		Intersection result{true, true, {0, 0}};

		float minIntervalDistance = std::numeric_limits<float>::infinity();
		sf::Vector2f translationAxis, edge, centerA = a.getCenter(), centerB = b.getCenter();

		for (int edgeIndex = 0; edgeIndex < a.getEdgeCount() + b.getEdgeCount(); edgeIndex++)
		{
			if (edgeIndex < a.getEdgeCount())
				edge = a.getEdge(edgeIndex);
			else
				edge = b.getEdge(edgeIndex - a.getEdgeCount());

			sf::Vector2f axis = sf::Vector2f(-edge.y, edge.x);
			math::normalize(axis);

			float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
			math::projectPolygon(axis, a, minA, maxA);
			math::projectPolygon(axis, b, minB, maxB);

			if (math::intervalDistance(minA, maxA, minB, maxB) >= 0)
				result.intersect = false;

			float velocityProjection = math::dotProduct<float>(axis, velocity);

			if (velocityProjection < 0)
				minA += velocityProjection;
			else
				maxA += velocityProjection;

			float intervalDistance = math::intervalDistance(minA, maxA, minB, maxB);

			if (intervalDistance >= 0)
				result.willIntersect = false;

			if (!MTV)
				return result;

			if (!result.intersect && !result.willIntersect)
				break;

			intervalDistance = std::abs(intervalDistance);

			if (intervalDistance < minIntervalDistance)
			{
				minIntervalDistance = intervalDistance;
				translationAxis = axis;

				sf::Vector2f d = sf::Vector2f(centerA.x - centerB.x, centerA.y - centerB.y);
				if (math::dotProduct<float>(d, translationAxis) < 0)
					translationAxis = sf::Vector2f(-translationAxis.x, -translationAxis.y);
			}
		}

		if (result.willIntersect)
			result.minimumTranslationVector = sf::Vector2f(translationAxis.x*minIntervalDistance, translationAxis.y*minIntervalDistance);

		return result;
	}
}

