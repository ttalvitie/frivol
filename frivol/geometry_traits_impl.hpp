#ifndef FRIVOL_GEOMETRY_TRAITS_IMPL_HPP
#define FRIVOL_GEOMETRY_TRAITS_IMPL_HPP

#include <cmath>
#include <limits>

namespace frivol {

template <typename CoordT>
CoordT GeometryTraitsFloat<CoordT>::getBreakpointX(
	const Point<CoordT>& a,
	const Point<CoordT>& b,
	CoordT topy
) {
	if(a.y == topy) {
		return a.x;
	}
	if(b.y == topy) {
		return b.x;
	}
	
	// Naive equation solution.
	CoordT x1;
	{
		// The equation is A x^2 + B x + C = 0, where
		// TODO: clarify and optimize.
		CoordT A = a.y-b.y;
		CoordT B = -2*a.y*b.x+2*topy*b.x+2*a.x*b.y-2*topy*a.x;
		CoordT C = 
			-topy*b.x*b.x-topy*b.y*b.y+topy*topy*b.y-a.x*a.x*b.y+a.x*a.x*topy
			-a.y*topy*topy-a.y*a.y*b.y+a.y*b.x*b.x+a.y*b.y*b.y+a.y*a.y*topy;
		
		// Choose the right solution: if a.y < b.y, the smaller solution and
		// otherwise the greater solution.
		CoordT sign = (a.y < b.y == A > 0.0) ? -1.0 : 1.0;
		
		x1 = (-B + sign * std::sqrt(B * B - 4 * A * C)) / (2 * A);
	}
	
	// Choosing the average of the x coordinates of the sites, exact in the
	// case where a.y == b.y.
	CoordT x2 = 0.5 * (a.x + b.x);
	
	// If x1 is not finite, x2 has to be better.
	if(!std::isfinite(x1)) return x2;
	
	// Choose the better matching solution.
	
	// Function for calculating the y coordinate difference of the parabolas in
	// given X coordinate.
	auto diff = [=](CoordT x) {
		CoordT a_y = 0.5*(-x*x+2*x*a.x-a.x*a.x-a.y*a.y+topy*topy)/(-a.y+topy);
		CoordT b_y = 0.5*(-x*x+2*x*b.x-b.x*b.x-b.y*b.y+topy*topy)/(-b.y+topy);
		return a_y - b_y;
	};
	
	if(std::abs(diff(x1)) < std::abs(diff(x2))) {
		return x1;
	} else {
		return x2;
	}
}

template <typename CoordT>
CoordT GeometryTraitsFloat<CoordT>::getCircumcircleTopY(
	const Point<CoordT>& a,
	const Point<CoordT>& b,
	const Point<CoordT>& c
) {
	// TODO: clarify and optimize.
	CoordT x = 0.5 * (
			-a.y*b.x*b.x+b.x*b.x*c.y-b.y*c.y*c.y+b.y*a.y*a.y+a.y*c.x*c.x-b.y*b.y*a.y
			-b.y*c.x*c.x+a.x*a.x*b.y-c.y*a.y*a.y+b.y*b.y*c.y-a.x*a.x*c.y+c.y*c.y*a.y
		) / (
			a.x*b.y-a.x*c.y-a.y*b.x+a.y*c.x+b.x*c.y-b.y*c.x
		);
	CoordT y = -0.5 * (
			-a.x*b.x*b.x-a.x*b.y*b.y+a.x*c.x*c.x+a.x*c.y*c.y+a.x*a.x*b.x-a.x*a.x*c.x
			+a.y*a.y*b.x-a.y*a.y*c.x-b.x*c.x*c.x-b.x*c.y*c.y+b.x*b.x*c.x+b.y*b.y*c.x
		) / (
			a.x*b.y-a.x*c.y-a.y*b.x+a.y*c.x+b.x*c.y-b.y*c.x
		);
	
	CoordT dx = x - a.x;
	CoordT dy = y - a.y;
	CoordT ret = y + std::sqrt(dx * dx + dy * dy);
	
	// In case of 0/0, return infinity.
	if(std::isnan(ret)) ret = std::numeric_limits<CoordT>::infinity();
	
	return ret;
}

template <typename CoordT>
bool GeometryTraitsFloat<CoordT>::isCCW(
	const Point<CoordT>& a,
	const Point<CoordT>& b,
	const Point<CoordT>& c
) {
	// Orientation can be seen from the sign of the cross product of two sides.
	CoordT dx1 = b.x - a.x;
	CoordT dy1 = b.y - a.y;
	CoordT dx2 = c.x - a.x;
	CoordT dy2 = c.y - a.y;
	
	return dx1 * dy2 > dx2 * dy1;
}

}

#endif
