#ifndef FRIVOL_GEOMETRY_TRAITS_HPP
#define FRIVOL_GEOMETRY_TRAITS_HPP

#include "priority_queue_concept.hpp"
#include "search_tree_concept.hpp"
#include "point.hpp"

namespace frivol {

/// Traits class that gives needed geometry operations for the algorithm.
/// Implemented traits are required by Policy.
/// @tparam CoordT The coordinate type to use.
template <typename CoordT>
struct GeometryTraits { };

/// Concept for checking that all required GeometryTraits are implemented for
/// given coordinate type. Required operations are:
///  - CoordT getBreakpointX(Point<CoordT> a, Point<CoordT> b, CoordT topy)
///    returns the X coordinate of intersection of the two parabolas defined by
///    (x-a.x)^2 + (y-a.y)^2 = (y-topy)^2 = (x-b.x)^2 + (y-b.y)^2
///    The result should be between a.x and b.x, a.x <= b.x.
/// 
/// @tparam CoordT The coordinate type.
template <typename CoordT>
class GeometryTraitsImplementedConcept {
public:
	BOOST_CONCEPT_USAGE(GeometryTraitsImplementedConcept) {
		typedef GeometryTraits<CoordT> Traits;
		sameType(Traits::getBreakpointX(point, point, coord), coord);
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
	static double getBreakpointX(
		const Point<CoordT>& a,
		const Point<CoordT>& b,
		CoordT topy
	) {
		// TODO: implement
		return 0.5 * (a.x + b.x);
	}
};

template <>
struct GeometryTraits<float> : GeometryTraitsFloat<float> { };
template <>
struct GeometryTraits<double> : GeometryTraitsFloat<double> { };

}

#endif
