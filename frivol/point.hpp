#ifndef FRIVOL_POINT_HPP
#define FRIVOL_POINT_HPP

namespace frivol {

/// Two-dimensional point.
/// @tparam CoordT The coordinate type to use. Should be default constructible.
/// Defaults to double, which is the coordinate type of DefaultPolicy.
template <typename CoordT = double>
struct Point {
	/// Constructs point with given coordinates.
	/// @param x The X coordinate.
	/// @param y The Y coordinate.
	Point(CoordT x, CoordT y) : x(x), y(y) { }
	
	/// Constructs point with undefined values as coordinates.
	Point() { }
	
	CoordT x; ///< The X coordinate of the point.
	CoordT y; ///< The Y coordinate of the point.
};

}

#endif
