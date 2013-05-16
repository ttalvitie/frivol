#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


#include "frivol/frivol.hpp"

using namespace frivol;

BOOST_AUTO_TEST_SUITE(tmp_test)

BOOST_AUTO_TEST_CASE(tmp_frivol_test) {
	Array<Point<>> points(3);
	points[0] = Point<>(-1, 0);
	points[1] = Point<>(1, 0);
	points[2] = Point<>(0, 1);
	computeVoronoiDiagram(points);
}

BOOST_AUTO_TEST_SUITE_END()
