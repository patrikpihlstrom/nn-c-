#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <math.h>
#include <cmath>
#include <limits>
#include <vector>
#include <cassert>
#include <iostream>


namespace math
{

	template <typename T>
	float distance(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
	}

	template <typename T>
	float angle(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
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
		return a.x*b.x + a.y*b.y;
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

		/*vector.x = std::cos(angle)*(vector.x - anchor.x) - std::sin(angle)*(vector.y - anchor.y) + anchor.x;
		vector.y = std::sin(angle)*(vector.x - anchor.x) + std::cos(angle)*(vector.y - anchor.y) + anchor.y;*/
	}

	template <typename T>
	void extendSegment(const sf::Vector2<T>& a, sf::Vector2<T>& b, const T extension)
	{
		float length = std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
		sf::Vector2<T> c{b.x + (b.x - a.x)/length*extension, b.y + (b.y - a.y)/length*extension};
		b = c;
	}

	template <typename T>
	std::vector<sf::Vector2<T>> ConvexHull(std::vector<sf::Vector2<T>> & p_points)
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
	 
		H.resize(k);
		return H;
	}

	template <typename T>
	struct Segment
	{
		sf::Vector2<T> a, b;
	};

	class Polygon
	{
	private:
		std::vector<sf::Vector2f> m_points, m_edges;

	public:
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
		}

		int getPointCount() const
		{
			return m_points.size();
		}

		int getEdgeCount() const
		{
			return m_edges.size();
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

		sf::Vector2f getCenter() const
		{
			float x = 0, y = 0;
			
			for (int i = 0; i < m_points.size(); ++i)
			{
				x += m_points[i].x;
				y += m_points[i].y;
			}

			return sf::Vector2f{x/(float)m_points.size(), y/(float)m_points.size()};
		}

		void offset(const float& x, const float& y)
		{
			for (int i = 0; i < m_points.size(); ++i)
				m_points[i] = sf::Vector2f{m_points[i].x + x, m_points[i].y + y};
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

		sf::Rect<int> getBounds() const
		{
			if (m_points.empty())
				return sf::Rect<int>(0, 0, 0, 0);

			sf::Rect<int> rectangle(m_points[0].x, m_points[0].y, m_points[0].x, m_points[0].y);

			for (int i = 0; i < m_points.size(); ++i)
			{
				if (m_points[i].x < rectangle.left)
					rectangle.left = m_points[i].x;
				
				if (m_points[i].y < rectangle.top)
					rectangle.top = m_points[i].y;

				if (m_points[i].x > rectangle.left)
					rectangle.width = m_points[i].x;
				
				if (m_points[i].y > rectangle.top)
					rectangle.height = m_points[i].y;
			}

			rectangle.width -= rectangle.left;
			rectangle.height -= rectangle.top;

			return rectangle;
		}
	};

	bool lineIntersectsLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d);
	bool lineIntersectsLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d, sf::Vector2f& e);
	bool lineIntersectsPolygon(const sf::Vector2f& a, const sf::Vector2f& b, sf::Vector2f& c, const Polygon& polygon);
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

