#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


#include "frivol/frivol.hpp"

using namespace frivol;

BOOST_AUTO_TEST_SUITE(tmp_test)

BOOST_AUTO_TEST_CASE(tmp_frivol_test) {
	Array<Point<>> points(5);
	computeVoronoiDiagram(points);
}

BOOST_AUTO_TEST_SUITE_END()
