#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <cmath>
#include <limits>
#include <vector>
#include <cassert>
#include <iostream>


namespace math
{
	template <typename T>
	sf::Rect<T> getBoundingBox(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return sf::Rect<T>{a.x < b.x ? a.x:b.x, a.y < b.y ? a.y:b.y, std::abs(a.x - b.x), std::abs(a.y - b.y)};
	}

	std::vector<sf::Vector2f> cutTri(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d, const sf::Vector2f& e);
	template <typename T>
	float distance(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
	}

	template <typename T>
	static float angle(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return std::atan2(a.y - b.y, a.x - b.x);
	}

	template <typename T>
	T signed2DTriArea(const sf::Vector2<T> a, const sf::Vector2<T> b, const sf::Vector2<T> c)
	{
		return (T)((a.x - c.x)*(b.y - c.y) - (a.y - c.y)*(b.x - c.x));
	}

	template <typename T>
	T toDegrees(const T& radians)
	{
		return T(radians*(180/M_PI));
	}

	template <typename T>
	T toRadians(const T& degrees)
	{
		return T(degrees*(M_PI/180));
	}

	template <typename T>
	T dotProduct(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return (T)((T)a.x*(T)b.x + (T)a.y*(T)b.y);
	}

	template <typename T>
	T magnitude(const sf::Vector2<T>& vector)
	{
		return (T)(std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2)));
	}

	template <typename T>
	T cross(const sf::Vector2<T>& o, const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return ((a.x - o.x)*(b.y - o.y) - (a.y - o.y)*(b.x - o.x));
	}

	template <typename T>
	bool lessThan(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return (a.x < b.x) ? a.x < b.x:(a.x == b.x ? a.y < b.y:false);
	}

	template <typename T>
	bool greaterThan(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return (a.x > b.x) ? a.x > b.x:(a.x == b.x ? a.y > b.y:false);
	}

	template <typename T>
	void rotateVector(const sf::Vector2<T>& anchor, sf::Vector2<T>& vector, const float angle)
	{
		float s = std::sin(angle), c = std::cos(angle);

		vector.x -= anchor.x;
		vector.y -= anchor.y;

		float x = vector.x*c - vector.y*s;
		float y = vector.x*s + vector.y*c;

		vector.x = x + anchor.x;
		vector.y = y + anchor.y;
	}

	template <typename T>
	void extendSegment(const sf::Vector2<T>& a, sf::Vector2<T>& b, const T extension)
	{
		float length = std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
		sf::Vector2<T> c{b.x + (b.x - a.x)/length*extension, b.y + (b.y - a.y)/length*extension};
		b = c;
	}

	template <typename T>
	int sign(const T& val)
	{
		return (T(0) < val) - (val < T(0));
	}

	template <typename T>
	void swap(T& a, T& b)
	{
		T c = a;
		a = b;
		b = c;
	}

	template <typename T>
	std::vector<sf::Vector2<T>> convexHull(std::vector<sf::Vector2<T>> & p_points)
	{
		int n = p_points.size();
		int k = 0;

		std::vector<sf::Vector2<T>> H(2*n);

		std::sort(p_points.begin(), p_points.end(), lessThan<T>);

		//lower
		for (int i = 0; i < n; ++i)
		{
			while(k >= 2 && cross(H[k-2], H[k-1], p_points[i]) <= 0)
			{
			    k--;
			}
			H[k++] = p_points[i];
		}


		//upper
		for (int i = n-2, t = k+1; i >= 0; i--) {
			while (k >= t && cross(H[k-2], H[k-1], p_points[i]) <= 0)
			{
				k--;
			}
			H[k++] = p_points[i];
		}
	 
		H.resize(k - 1);

		return H;
	}
	
	static bool lineIntersectsLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d)
	{
		float q = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
		float D = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);

		if (D == 0)
			return false;

		float r = q/D;
		q = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);
		float s = q/D;

		if (r < 0 || r > 1 || s < 0 || s > 1)
			return false;

		return true;
	}

	static bool lineIntersectsLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d, sf::Vector2f& e)
	{
		float a1 = signed2DTriArea<float>(a, b, d);
		float a2 = signed2DTriArea<float>(a, b, c);

		if (a1*a2 < 0.0f)
		{
			float a3 = signed2DTriArea<float>(c, d, a);
			float a4 = a3 + a2 - a1;

			if (a3*a4 < 0.0f)
			{
				float t = a3/(a3 - a4);
				e = sf::Vector2f(a.x + t*(b.x - a.x), a.y + t*(b.y - a.y));
				return true;
			}
		}

		return false;
	}

	template <typename T>
	float isLeft(const sf::Vector2<T>& a, const sf::Vector2<T>& b, const sf::Vector2<T>& c)
	{
		return (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
	}

	template <typename T>
	bool above(const sf::Vector2<T>& a, const sf::Vector2<T>& b, const sf::Vector2<T>& c)
	{
		return isLeft<T>(a, b, c) > 0;
	}

	template <typename T>
	bool below(const sf::Vector2<T>& a, const sf::Vector2<T>& b, const sf::Vector2<T>& c)
	{
		return isLeft<T>(a, b, c) < 0;
	}

	sf::Vector2f getLineIntersection(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d, bool& interrupted);

	template <typename T>
	struct Segment
	{
		sf::Vector2<T> a, b;
	};

	class Polygon
	{
	private:
		std::vector<sf::Vector2f> m_points, m_edges;
		sf::Vector2f m_position;
		float m_depth;

	public:
		std::vector<sf::Vector2f> getPoints() const
		{
			return m_points;
		}

		void constructEdges()
		{
			sf::Vector2f a, b;
			m_edges.clear();

			for (int i = 0; i < m_points.size(); ++i)
			{
				a = m_points[i];

				if (i + 1 >= m_points.size())
					b = m_points[0];
				else
					b  = m_points[i + 1];

				m_edges.push_back(sf::Vector2f{b.x - a.x, b.y - a.y});
			}

			m_position = getCenter();
		}

		int getPointCount() const
		{
			return m_points.size();
		}

		int getEdgeCount() const
		{
			return m_edges.size();
		}

		float getDepth() const
		{
			return m_depth;
		}

		void setDepth(const float& depth)
		{
			m_depth = depth;
		}

		sf::Vector2f getPoint(const int& index) const
		{
			assert(index < m_points.size());
			return m_points[index];
		}

		sf::Vector2f getEdge(const int& index) const
		{
			assert(index < m_edges.size());
			return m_edges[index];
		}

		template <typename T>
		Segment<T> getEdgeSegment(const int& index) const
		{
			assert(index < m_points.size());
			
			if (index < m_points.size() - 1)
				return {m_points[index], m_points[index + 1]};
			else
				return {m_points[index], m_points[0]};
		}

		sf::Vector2f getPosition() const
		{
			return m_position;
		}

		sf::Vector2f getCenter() const
		{
			auto bounds = getBounds();
			return sf::Vector2f{(float)bounds.left + (bounds.width/2), (float)bounds.top + (bounds.height/2)};
		}

		void offset(const float& x, const float& y)
		{
			for (int i = 0; i < m_points.size(); ++i)
				m_points[i] = sf::Vector2f{m_points[i].x + x, m_points[i].y + y};
			m_position.x += x;
			m_position.y += y;
		}

		void addPoint(const sf::Vector2f& point)
		{
			m_points.push_back(point);
		}

		void clear()
		{
			m_points.clear();
			m_edges.clear();
		}

		void convexHull()
		{
			m_points = math::convexHull(m_points);
		}

		sf::Rect<int> getBounds() const
		{
			if (m_points.empty())
				return sf::Rect<int>(0, 0, 0, 0);

			sf::Vector2f min = m_points[0], max = m_points[0];

			for (int i = 0; i < m_points.size(); ++i)
			{
				if (m_points[i].x < min.x)
					min.x = m_points[i].x;
				
				if (m_points[i].y < min.y)
					min.y = m_points[i].y;

				if (m_points[i].x > max.x)
					max.x = m_points[i].x;
				
				if (m_points[i].y > max.y)
					max.y = m_points[i].y;
			}

			sf::Rect<int> rectangle;
			rectangle.left = min.x;
			rectangle.top = min.y;
			rectangle.width = max.x - min.x;
			rectangle.height = max.y - min.y;

			return rectangle;
		}

		void setPoints(const std::vector<sf::Vector2f>& points)
		{
			m_points = points;
		}

		bool containsPoint(const sf::Vector2f& point) const
		{
			auto bounds = getBounds();
			sf::Vector2f _point{point.x, (float)bounds.top - 10};

			int intersections = 0;
			for (int i = 0; i < getEdgeCount(); ++i)
			{
				auto segment = getEdgeSegment<float>(i);
				if (lineIntersectsLine(point, _point, segment.a, segment.b))
					intersections++;
			}

			return !(intersections%2 == 0);
		}

		enum class LineIntersection : char
		{
			None,
			A,
			B,
			Both,
		};

		LineIntersection intersectsLine(const sf::Vector2f& a, const sf::Vector2f& b, sf::Vector2f& c, sf::Vector2f& d) const
		{
			bool C = false, D = false;

			for (int i = 0; i < getEdgeCount(); ++i)
			{
				auto segment = getEdgeSegment<float>(i);

				if (!C)
				{
					if (lineIntersectsLine(a, b, segment.a, segment.b, c))
						C = true;
				}

				if (!D)
				{
					if (lineIntersectsLine(a, b, segment.a, segment.b, d))
						D = true;
				}

				if (C && D)
					break;
			}

			LineIntersection result;

			if (C)
			{
				if (D)
					result = LineIntersection::Both;
				else
					result = LineIntersection::A;
			}
			else if (D)
				result = LineIntersection::B;
			else
				result = LineIntersection::None;

			return result;
		}

		void removeDoubles()
		{
			std::vector<sf::Vector2f> points;

			for (int i = 0; i < m_points.size(); ++i)
			{
				if (std::find(points.begin(), points.end(), m_points[i]) == points.end())
					points.push_back(m_points[i]);
				else
					std::cout << m_points[i].x << ':' << m_points[i].y << std::endl;
			}

			m_points = points;
		}

		Polygon operator& (const Polygon& polygon) const
		{
			Polygon _polygon;
			std::vector<sf::Vector2f> points;
			sf::Vector2f point;
			
			for (int i = 0, j = 0; i < getEdgeCount(); ++i)
			{
				auto edge = getEdgeSegment<float>(i);
				auto extendedEdge = getEdgeSegment<float>(i);
				extendSegment<float>(extendedEdge.a, extendedEdge.b, .1f);
				extendSegment<float>(extendedEdge.b, extendedEdge.a, .1f);

				bool a = polygon.containsPoint(extendedEdge.a), b = polygon.containsPoint(extendedEdge.b);
				if (a)
					points.push_back(edge.a);

				if (b)
					points.push_back(edge.b);

				if (a && b)
					continue;

				for (j = 0; j < polygon.getEdgeCount(); ++j)
				{
					auto _edge = polygon.getEdgeSegment<float>(j);
					auto _extendedEdge = polygon.getEdgeSegment<float>(j);
					extendSegment<float>(_extendedEdge.a, _extendedEdge.b, .1);
					extendSegment<float>(_extendedEdge.b, _extendedEdge.a, .1);


					if (lineIntersectsLine(extendedEdge.a, extendedEdge.b, _extendedEdge.a, _extendedEdge.b, point))
						points.push_back(point);
					else
					{
						if (containsPoint(_extendedEdge.a))
							points.push_back(_edge.a);
						
						if (containsPoint(_extendedEdge.b))
							points.push_back(_edge.b);
					}
				}
			}

			if (!points.empty())
			{
				points = math::convexHull(points);
				_polygon.setPoints(points);
				_polygon.constructEdges();
			}

			return _polygon;
		}

		void rotate(const float angle)
		{
			auto anchor = getCenter();
			for (int i = 0; i < m_points.size(); ++i)
			{
				math::rotateVector<float>(anchor, m_points[i], math::toRadians<float>(angle));
			}

			constructEdges();
		}

		std::vector<short> projectPivot(const sf::Vector2f& axis, const sf::Vector2f& point) const
		{
			assert(!m_points.empty());

			std::vector<short> result;
			result.reserve(m_points.size());

			float pivot = math::dotProduct<float>(axis, point);

			for (int i = 0; i < m_points.size(); ++i)
			{
				float dotProduct = math::dotProduct<float>(axis, m_points[i]);
				if (dotProduct < pivot)
					result.push_back(-1);
				else if (dotProduct > pivot)
					result.push_back(1);
				else
					result.push_back(0);
			}

			return result;
		}
	};

	bool lineIntersectsPolygon(const sf::Vector2f& a, const sf::Vector2f& b, const Polygon& polygon);
	bool lineIntersectsPolygon(const sf::Vector2f& a, const sf::Vector2f& b, sf::Vector2f& c, sf::Vector2f& d, const Polygon& polygon);
	bool pointInPolygon(const sf::Vector2f& point, const Polygon& polygon);

	struct Intersection
	{
		bool intersect;
		bool willIntersect;
		sf::Vector2f minimumTranslationVector;
	};

	void normalize(sf::Vector2f& vector);
	void projectPolygon(const sf::Vector2f& axis, const Polygon& polygon, float& min, float& max);
	float intervalDistance(const float& minA, const float& maxA, const float& minB, const float& maxB);
	bool polygonIntersectsPolygon(const Polygon& a, const Polygon& b);
	Intersection SAT(const Polygon& a, const Polygon& b, const sf::Vector2f& velocity, const bool MTV = true);
}

