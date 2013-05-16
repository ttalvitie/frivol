#include <boost/test/unit_test.hpp>

#include "frivol/geometry_traits.hpp"
#include "frivol/point.hpp"

using namespace frivol;

typedef GeometryTraits<float> TraitsT;
typedef Point<float> PointT;

float eps = 0.001;

BOOST_AUTO_TEST_SUITE(geometry_traits_float)

BOOST_AUTO_TEST_CASE(breakpoint_horizontal_sites) {
	PointT a(0, 0);
	PointT b(1, 0);
	float x = 0.5;
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 0.001), x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 0.3), x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 1), x, eps);
}
BOOST_AUTO_TEST_CASE(breakpoint_almost_horizontal_sites) {
	PointT a(0, 0);
	PointT b(1, 0.05 * eps);
	float x = 0.5;
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 0.3), x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 1), x, eps);
}

BOOST_AUTO_TEST_CASE(breakpoint_vertical_sites) {
	PointT a(1, 0);
	PointT b(1, 1);
	
	// If topy = 2, the parabolas are y = (3-x^2)/2 and y = (1-x^2/4).
	// Therefore the intersections are x = 1+-sqrt(2).
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 2), 1 - std::sqrt(2.0), eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(b, a, 2), 1 + std::sqrt(2.0), eps);
}

BOOST_AUTO_TEST_CASE(equal_breakpoints) {
	PointT a(1, 3);
	
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, a, 3), a.x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, a, 3.1), a.x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, a, 4), a.x, eps);
}

BOOST_AUTO_TEST_SUITE_END()
