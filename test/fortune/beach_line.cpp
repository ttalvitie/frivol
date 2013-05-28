#include <boost/test/unit_test.hpp>

#include <frivol/policy.hpp>
#include <frivol/fortune/beach_line.hpp>

using namespace frivol;

typedef fortune::BeachLine<DefaultPolicy> BeachLineT;

// Some parts of beach line are tested as a part of fortune_algorithm.
BOOST_AUTO_TEST_SUITE(beach_line)

BOOST_AUTO_TEST_CASE(max_arc_count_works) {
	containers::Array<Point<>> sites(3);
	BeachLineT beach_line(sites, 51);
	BOOST_CHECK_EQUAL(beach_line.getMaxArcCount(), 51);
}

BOOST_AUTO_TEST_CASE(empty_insert_works) {
	containers::Array<Point<>> sites(1);
	BeachLineT beach_line(sites, 1);
	BOOST_CHECK_EQUAL(beach_line.insertArc(0, 0), 0); // Only possible arc id.
}

BOOST_AUTO_TEST_CASE(arc_tower_works) {
	// "tower" of three arcs
	containers::Array<Point<>> sites(3);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(0, 1);
	sites[2] = Point<>(0, 2);
	BeachLineT beach_line(sites, 5);
	
	beach_line.insertArc(0, 0);
	beach_line.insertArc(1, 1);
	Idx middle = beach_line.insertArc(2, 2);
	
	// Check that all arcs have the right origins and there are no extra
	// arcs.
	BOOST_CHECK_EQUAL(beach_line.getOriginSite(middle), 2);
	
	Idx left = beach_line.getLeftArc(middle);
	BOOST_CHECK_EQUAL(beach_line.getOriginSite(left), 1);
	left = beach_line.getLeftArc(left);
	BOOST_CHECK_EQUAL(beach_line.getOriginSite(left), 0);
	left = beach_line.getLeftArc(left);
	BOOST_CHECK_EQUAL(left, nil_idx);
	
	Idx right = beach_line.getRightArc(middle);
	BOOST_CHECK_EQUAL(beach_line.getOriginSite(right), 1);
	right = beach_line.getRightArc(right);
	BOOST_CHECK_EQUAL(beach_line.getOriginSite(right), 0);
	right = beach_line.getRightArc(right);
	BOOST_CHECK_EQUAL(right, nil_idx);
}

BOOST_AUTO_TEST_CASE(arc_overflow_throws) {
	containers::Array<Point<>> sites(3);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(0, 1);
	sites[2] = Point<>(0, 2);
	BeachLineT beach_line(sites, 4);
	
	bool thrown = false;
	beach_line.insertArc(0, 0);
	beach_line.insertArc(1, 1);
	try {
		beach_line.insertArc(2, 2);
	} catch(std::logic_error& e) {
		thrown = true;
	}
	BOOST_CHECK(thrown);
}

BOOST_AUTO_TEST_SUITE_END()
