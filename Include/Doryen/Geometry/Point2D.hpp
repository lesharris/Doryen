#ifndef LIBTCOD_POINT2D_HPP
#define LIBTCOD_POINT2D_HPP

#include <string>
#include <cstdint>

namespace Doryen
{
	namespace Geometry
	{

		/**
		 * The Point2D class defines a point representing a location in (x,y)
		 * coordinate space.
		 *
		 * @tparam T Arithmetic type: Integer, Real
		 */
		template<typename T = std::int32_t>
		class Point2D
		{

		public:

			// Fields Public

			T x;
			T y;

			// Constructs

			Point2D() noexcept
			{
				x = T{};
				y = T{};
			}

			Point2D(T _x, T _y) noexcept
			{
				x = _x;
				y = _y;
			}

			// Methods

			/**
			 * Determines whether or not two points are equal. Two instances of
			 * Point2D are equal if the values of their x and y member fields,
			 * representing their position in the coordinate space, are the same.
			 *
			 * @param _rhs an object to be compared with this Point2D
			 *
			 * @return true if the object to be compared is an instance of
			 *  Point2D and has the same values; false otherwise.
			 */
			bool equals(const Point2D<T>& _rhs) const noexcept
			{
				return this->x == _rhs.x and this->y == _rhs.y;
			}

			// Debug

			[[maybe_unused]] std::string toString() const
			{
				return { '{' + std::to_string(x) + ", " + std::to_string(y) + '}' };
			}

			// Getters

			/**
			 * @return the X coordinate of this Point2D.
			 */
			T getX() const
			{
				return x;
			}

			/**
			 * @return the Y coordinate of this Point2D.
			 */
			T getY() const
			{
				return y;
			}

			// Setters

			/**
			 * @param _x the new X coordinate of this Point2D
			 */
			void setX(T _x)
			{
				x = _x;
			}

			/**
			 * @param _y the new Y coordinate of this Point2D
			 */
			void setY(T _y)
			{
				y = _y;
			}

		};
    }
}

#endif //LIBTCOD_POINT2D_HPP
