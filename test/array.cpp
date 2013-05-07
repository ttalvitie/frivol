#include <boost/test/unit_test.hpp>

#include "frivol/array.hpp"

using namespace frivol;

BOOST_AUTO_TEST_SUITE(array)

BOOST_AUTO_TEST_CASE(size_works) {
	Array<double> array(7);
	BOOST_CHECK_EQUAL(array.getSize(), 7);
}

BOOST_AUTO_TEST_CASE(simple_read_write) {
	Array<int> array(3);
	array[0] = 3;
	array[2] = -3;
	array[1] = 2;
	BOOST_CHECK_EQUAL(array[0], 3);
	BOOST_CHECK_EQUAL(array[1], 2);
	BOOST_CHECK_EQUAL(array[2], -3);
}

BOOST_AUTO_TEST_SUITE_END()
