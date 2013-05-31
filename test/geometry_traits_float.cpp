#include <boost/test/unit_test.hpp>

#include <frivol/geometry_traits.hpp>
#include <frivol/point.hpp>

using namespace frivol;

typedef GeometryTraits<float> TraitsT;
typedef Point<float> PointT;

float eps = 0.005;

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

BOOST_AUTO_TEST_CASE(breakpoint_right_infinity) {
	PointT a(0, 0);
	PointT b(1, 0);
	
	BOOST_CHECK(TraitsT::getBreakpointX(b, a, 3, false) < -100000.0);
	BOOST_CHECK(TraitsT::getBreakpointX(b, a, 3, true) > 100000.0);
}

BOOST_AUTO_TEST_CASE(breakpoint_basic) {
	PointT a(3, 5);
	PointT b(10, 3);
	float y = 11;
	
	// Solved (x-3)^2 + (y-5)^2 = (y-11)^2 = (x-10)^2 + (y-3)^2 with Maple
	// to obtain the following solution.
	BOOST_CHECK_CLOSE(TraitsT::getBreakpointX(a, b, y, false), 7.219, eps);
}

void checkPointsClose(PointT a, PointT b) {
	BOOST_CHECK_CLOSE(a.x, b.x, eps);
	BOOST_CHECK_CLOSE(a.y, b.y, eps);
}

BOOST_AUTO_TEST_CASE(circumcenter_basic) {
	PointT a(0, 0);
	PointT b(1, 1);
	PointT c(0, 2);
	
	PointT p(0, 1);
	
	checkPointsClose(TraitsT::getCircumcenter(a, b, c), p);
}

BOOST_AUTO_TEST_CASE(circumcenter_basic2) {
	PointT p(4, 2);
	
	float angle_a = 1;
	float angle_b = 2.213;
	float angle_c = 0.5;
	
	PointT a(p.x + std::cos(angle_a), p.y + std::sin(angle_a));
	PointT b(p.x + std::cos(angle_b), p.y + std::sin(angle_b));
	PointT c(p.x + std::cos(angle_c), p.y + std::sin(angle_c));
	
	checkPointsClose(TraitsT::getCircumcenter(a, b, c), p);
}


BOOST_AUTO_TEST_CASE(circumcenter_basic3) {
	PointT p(-2, 3.5);
	
	float angle_a = 1;
	float angle_b = 1.4;
	float angle_c = 3.24;
	
	PointT a(p.x + std::cos(angle_a), p.y + std::sin(angle_a));
	PointT b(p.x + std::cos(angle_b), p.y + std::sin(angle_b));
	PointT c(p.x + std::cos(angle_c), p.y + std::sin(angle_c));
	
	checkPointsClose(TraitsT::getCircumcenter(a, b, c), p);
}

BOOST_AUTO_TEST_CASE(circumcircle_topy_works) {
	PointT p(-2, 3.5);
	
	float angle_a = 1;
	float angle_b = 1.4;
	float angle_c = 3.24;
	
	PointT a(p.x + std::cos(angle_a), p.y + std::sin(angle_a));
	PointT b(p.x + std::cos(angle_b), p.y + std::sin(angle_b));
	PointT c(p.x + std::cos(angle_c), p.y + std::sin(angle_c));
	
	BOOST_CHECK_CLOSE(TraitsT::getCircumcircleTopY(a, b, c), p.y + 1, eps);
}

BOOST_AUTO_TEST_SUITE_END()
