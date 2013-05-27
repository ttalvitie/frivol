#include <boost/test/unit_test.hpp>

#include "frivol/containers/dynamic_array.hpp"

using namespace frivol;
using namespace frivol::containers;

BOOST_AUTO_TEST_SUITE(dynamic_array)

BOOST_AUTO_TEST_CASE(constructed_size_works) {
	DynamicArray<double> array(7);
	BOOST_CHECK_EQUAL(array.getSize(), 7);
}

BOOST_AUTO_TEST_CASE(default_constructed_size_works) {
	DynamicArray<double> array;
	BOOST_CHECK_EQUAL(array.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(simple_read_write) {
	DynamicArray<int> array(3);
	array[0] = 3;
	array[2] = -3;
	array[1] = 2;
	BOOST_CHECK_EQUAL(array[0], 3);
	BOOST_CHECK_EQUAL(array[1], 2);
	BOOST_CHECK_EQUAL(array[2], -3);
}

BOOST_AUTO_TEST_CASE(add_increases_size) {
	DynamicArray<double> array(4);
	array.add(53);
	BOOST_CHECK_EQUAL(array.getSize(), 5);
}

BOOST_AUTO_TEST_CASE(large_add_works) {
	DynamicArray<int> array;
	for(int i = 0; i < 421; ++i) {
		array.add(i);
	}
	BOOST_CHECK_EQUAL(array.getSize(), 421);
	for(int i = 0; i < 421; ++i) {
		BOOST_CHECK_EQUAL(array[i], i);
	}
}

BOOST_AUTO_TEST_SUITE_END()
