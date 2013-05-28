#include <boost/test/unit_test.hpp>

#include "frivol/containers/array.hpp"

using namespace frivol;
using namespace frivol::containers;

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

BOOST_AUTO_TEST_CASE(resize_increases_size) {
	Array<bool> array(5);
	array.resize(7);
	BOOST_CHECK_EQUAL(array.getSize(), 7);
}

BOOST_AUTO_TEST_CASE(resize_decreases_size) {
	Array<bool> array(7);
	array.resize(5);
	BOOST_CHECK_EQUAL(array.getSize(), 5);
}

BOOST_AUTO_TEST_CASE(shrinking_resize_works) {
	Array<int> array(4);
	for(int i = 0; i < 4; ++i) {
		array[i] = 5 * i;
	}
	array.resize(3);
	for(int i = 0; i < 3; ++i) {
		BOOST_CHECK_EQUAL(array[i], 5 * i);
	}
}

BOOST_AUTO_TEST_CASE(growing_resize_works) {
	Array<int> array(4);
	for(int i = 0; i < 4; ++i) {
		array[i] = 43 * i;
	}
	array.resize(6);
	for(int i = 0; i < 4; ++i) {
		BOOST_CHECK_EQUAL(array[i], 43 * i);
	}
}

BOOST_AUTO_TEST_CASE(empty_array_works) {
	Array<double> array(0);
	BOOST_CHECK_EQUAL(array.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(default_constructor_works) {
	Array<double> array;
	BOOST_CHECK_EQUAL(array.getSize(), 0);
	array.resize(5);
	BOOST_CHECK_EQUAL(array.getSize(), 5);
}

BOOST_AUTO_TEST_SUITE_END()
