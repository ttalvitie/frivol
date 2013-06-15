#include <cmath>
#include <limits>

namespace frivol {

template <typename CoordT>
CoordT GeometryTraitsFloat<CoordT>::getBreakpointX(
	const PointT& a,
	const PointT& b,
	CoordT topy,
	bool positive_big
) {
	// Reduce to the case where a = (0, 0), b = (u, v), topy = h.
	CoordT u = b.x - a.x;
	CoordT v = b.y - a.y;
	CoordT h = topy - a.y;
	
	// If the points are on the same level, the result is either in the middle
	// or in infinity, because the parabolas meet only once.
	if(std::abs(v) < epsilon) {
		if(u > 0.0) {
			return 0.5 * (a.x + b.x);
		} else {
			if(positive_big) {
				return std::numeric_limits<CoordT>::infinity();
			} else {
				return -std::numeric_limits<CoordT>::infinity();
			}
		}
	}
	
	// To ensure numerical stability, handle degenerate cases separately.
	if(a.y > topy - epsilon) {
		return a.x;
	}
	if(b.y > topy - epsilon) {
		return b.x;
	}
	
	// Otherwise, we need to solve x from:
	//   x^2 + y^2 = (y - h)^2 = (x-u)^2 + (y-v)^2.
	// Subtracting first equality from the second yields
	//   2ux + 2vy = u^2 + v^2.
	// Solving y and substituting to the first equality yields
	//   A x^2 + B x + C = 0, where
	//   A = v
	//   B = -2hu
	//   C = h(u^2+v^2-vh).
	CoordT A = v;
	CoordT B = -2 * h * u;
	CoordT C = h * (u * u + v * v - v * h);
	
	CoordT discriminant = B * B - 4 * A * C;
	if(discriminant < 0.0) discriminant = 0.0;
	
	// If a is below b, we'll choose the leftmost solution and vice versa.
	// Therefore we'll subtract the square root in the quadratic formula.
	// In case the quadratic formula is numerically unstable, we use the
	// fact that C/A is the product of the two solutions.
	CoordT x;
	if(B > 0.0) {
		x = (-B - std::sqrt(discriminant)) / (2 * A);
	} else {
		x = 2 * C / (-B + std::sqrt(discriminant));
	}
	
	// Move the solution back to the original case.
	return a.x + x;
}

template <typename CoordT>
Point<CoordT> GeometryTraitsFloat<CoordT>::getCircumcenter(
	const PointT& a,
	const PointT& b,
	const PointT& c
) {
	// Reduce to case where a = 0, b = (x1, y1), c = (x2, y2)
	// We get equation (x-x1)^2 + (y-y1)^2 = x^2 + y^2 = (x-x2)^2 + (y-y2)^2.
	// Solving both equation signs yields x1*x + y1*y = (x1^2 + y1^2) / 2 and
	// similarly for x2 and y2.
	CoordT x1 = b.x - a.x;
	CoordT y1 = b.y - a.y;
	CoordT x2 = c.x - a.x;
	CoordT y2 = c.y - a.y;
	
	// Solving the pair of linear equations yields the following solution.
	CoordT divisor = 2 * (x1 * y2 - y1 * x2);
	CoordT x = (y1 * (y2 * y1 - x2 * x2) + y2 * (x1 * x1 - y2 * y1)) / divisor;
	CoordT y = (x1 * (y2 * y2 - x1 * x2) + x2 * (x1 * x2 - y1 * y1)) / divisor;
	
	return PointT(a.x + x, a.y + y);
}

template <typename CoordT>
CoordT GeometryTraitsFloat<CoordT>::getCircumcircleTopY(
	const PointT& a,
	const PointT& b,
	const PointT& c
) {
	PointT center = getCircumcenter(a, b, c);
	
	CoordT dx = center.x - a.x;
	CoordT dy = center.y - a.y;
	CoordT ret = center.y + std::sqrt(dx * dx + dy * dy);
	
	// In case of 0/0, return infinity.
	if(std::isnan(ret)) ret = std::numeric_limits<CoordT>::infinity();
	
	return ret;
}

template <typename CoordT>
bool GeometryTraitsFloat<CoordT>::isCCW(
	const PointT& a,
	const PointT& b,
	const PointT& c
) {
	// Orientation can be seen from the sign of the cross product of two sides.
	CoordT dx1 = b.x - a.x;
	CoordT dy1 = b.y - a.y;
	CoordT dx2 = c.x - a.x;
	CoordT dy2 = c.y - a.y;
	
	return dx1 * dy2 > dx2 * dy1;
}

}
