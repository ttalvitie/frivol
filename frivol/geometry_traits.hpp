#ifndef FRIVOL_GEOMETRY_TRAITS_HPP
#define FRIVOL_GEOMETRY_TRAITS_HPP

#include <frivol/containers/priority_queue_concept.hpp>
#include <frivol/containers/search_tree_concept.hpp>
#include <frivol/point.hpp>

namespace frivol {

/// Traits class that gives needed geometry operations for the algorithm.
/// Implemented traits are required by Policy.
/// @tparam CoordT The coordinate type to use.
template <typename CoordT>
struct GeometryTraits { };

/// Concept for checking that all required GeometryTraits are implemented for
/// given coordinate type. Required operations are:
///  - CoordT getBreakpointX(
///      Point<CoordT> a, Point<CoordT> b, CoordT topy, bool positive_big)
///    returns the X coordinate of intersection of the two parabolas defined by
///    (x-a.x)^2 + (y-a.y)^2 = (y-topy)^2 = (x-b.x)^2 + (y-b.y)^2
///    The function may assume that a.x <= b.x, a.y <= topy and b.y <= topy.
///    The function should choose the solution where the parabola around a
///    goes under the parabola around b. In cases where this does not happen,
///    the result should be very big number, positive if positive_big,
///    otherwise negative.
///  - CoordT getCircumcircleTopY(Point<CoordT> a, Point<CoordT> b, Point<CoordT> c)
///    returns the Y coordinate of the top point (i.e. highest Y coordinate) of
///    the circumscribed circle around triangle 'abc'. In case of (almost)
///    collinear points, the result should be very big or very small compared
///    to site coordinates.
///  - bool isCCW(Point<CoordT> a, Point<CoordT> b, Point<CoordT> c) returns true
///    if triangle 'abc' is oriented counterclockwise.
/// 
/// @tparam CoordT The coordinate type.
template <typename CoordT>
class GeometryTraitsImplementedConcept {
public:
	BOOST_CONCEPT_USAGE(GeometryTraitsImplementedConcept) {
		typedef GeometryTraits<CoordT> Traits;
		sameType(Traits::getBreakpointX(point, point, coord, true), coord);
		sameType(Traits::getCircumcircleTopY(point, point, point), coord);
		sameType(Traits::isCCW(point, point, point), bool());
	}
	
private:
	CoordT coord;
	Point<CoordT> point;
	
	/// Function that causes compile time error if called with parameters of
	/// different types.
	template <typename T>
	void sameType(const T&, const T&);
};

// Implementations of GeometryTraits for basic types.

/// Implementation of GeometryTraits for floating point coordinate types
/// (float and double).
template <typename CoordT>
struct GeometryTraitsFloat {
	// Small number that should be insignificant compared to site positions.
	static constexpr CoordT epsilon = 1e-30;
	
	static CoordT getBreakpointX(
		const Point<CoordT>& a,
		const Point<CoordT>& b,
		CoordT topy,
		bool positive_big
	);
	
	static CoordT getCircumcircleTopY(
		const Point<CoordT>& a,
		const Point<CoordT>& b,
		const Point<CoordT>& c
	);
	
	static bool isCCW(
		const Point<CoordT>& a,
		const Point<CoordT>& b,
		const Point<CoordT>& c
	);
};

template <typename CoordT>
constexpr CoordT GeometryTraitsFloat<CoordT>::epsilon;

template <>
struct GeometryTraits<float> : GeometryTraitsFloat<float> { };
template <>
struct GeometryTraits<double> : GeometryTraitsFloat<double> { };

}

#include "geometry_traits_impl.hpp"

#endif
