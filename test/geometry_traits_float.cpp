#include <boost/test/unit_test.hpp>

#include <frivol/geometry_traits.hpp>
#include <frivol/point.hpp>

using namespace frivol;

typedef GeometryTraits<float> TraitsT;
typedef Point<float> PointT;

float eps = 0.001;

BOOST_AUTO_TEST_SUITE(geometry_traits_float)

BOOST_AUTO_TEST_CASE(breakpoint_horizontal_sites) {
	PointT a(0, 0);
	PointT b(1, 0);
	float x = 0.5;
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 0.001, true), x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 0.3, false), x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 1, false), x, eps);
}
BOOST_AUTO_TEST_CASE(breakpoint_almost_horizontal_sites) {
	PointT a(0, 0);
	float x = 0.5;
	for(float d = 1e-30; d < 1e-6; d *= 10) {
		PointT b(1, d);
		BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 0.3, true), x, eps);
		BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 1, false), x, eps);
	}
}

BOOST_AUTO_TEST_CASE(breakpoint_vertical_sites) {
	PointT a(1, 0);
	PointT b(1, 1);
	
	// If topy = 2, the parabolas are y = (3-x^2)/2 and y = (1-x^2/4).
	// Therefore the intersections are x = 1+-sqrt(2).
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, 2, false), 1 - std::sqrt(2.0), eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(b, a, 2, true), 1 + std::sqrt(2.0), eps);
}

BOOST_AUTO_TEST_CASE(breakpoint_equal_sites) {
	PointT a(1, 3);
	
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, a, 3, false), a.x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, a, 3.1, false), a.x, eps);
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, a, 4, true), a.x, eps);
}

BOOST_AUTO_TEST_CASE(circumcenter_basic) {
	PointT a(0, 0);
	PointT b(1, 1);
	PointT c(0, 2);
	
	BOOST_CHECK_CLOSE(TraitsT::getCircumcircleTopY(a, b, c), 2, eps);
}

BOOST_AUTO_TEST_CASE(circumcenter2_basic) {
	PointT a(0, 0);
	PointT b(1, 0);
	PointT c(0.5, 1);
	
	BOOST_CHECK_CLOSE(TraitsT::getCircumcircleTopY(a, b, c), 1, eps);
}

BOOST_AUTO_TEST_CASE(circumcenter_collinear) {
	PointT a(0, 0);
	PointT b(0, 1);
	PointT c(0, 2);
	
	BOOST_CHECK(std::abs(TraitsT::getCircumcircleTopY(a, b, c)) > 10000.0);
}

BOOST_AUTO_TEST_CASE(circumcenter_two_incident) {
	PointT a(0, 0);
	PointT b(1, 1);
	
	BOOST_CHECK(std::abs(TraitsT::getCircumcircleTopY(a, b, b)) > 10000.0);
}

BOOST_AUTO_TEST_CASE(circumcenter_all_incident) {
	PointT a(5, 6);
	
	BOOST_CHECK(std::abs(TraitsT::getCircumcircleTopY(a, a, a)) > 10000.0);
}

BOOST_AUTO_TEST_SUITE_END()
