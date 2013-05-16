#include <boost/test/unit_test.hpp>

#include "frivol/stack.hpp"

using namespace frivol;

BOOST_AUTO_TEST_SUITE(stack)

BOOST_AUTO_TEST_CASE(basic_push_pop) {
	Stack<int> stack;
	stack.push(2);
	stack.push(5);
	stack.push(2);
	stack.push(7);
	BOOST_CHECK_EQUAL(stack.top(), 7);
	BOOST_CHECK_EQUAL(stack.top(), 7);
	stack.pop();
	BOOST_CHECK_EQUAL(stack.top(), 2);
	stack.push(-5);
	BOOST_CHECK_EQUAL(stack.top(), -5);
	stack.pop();
	BOOST_CHECK_EQUAL(stack.top(), 2);
	stack.pop();
	BOOST_CHECK_EQUAL(stack.top(), 5);
	stack.pop();
	BOOST_CHECK_EQUAL(stack.top(), 2);
	stack.pop();
}

BOOST_AUTO_TEST_CASE(empty_works) {
	Stack<int> stack;
	BOOST_CHECK(stack.empty());
	stack.push(0);
	BOOST_CHECK(!stack.empty());
	stack.pop();
	BOOST_CHECK(stack.empty());
	stack.push(0);
	BOOST_CHECK(!stack.empty());
	stack.push(0);
	BOOST_CHECK(!stack.empty());
	stack.pop();
	BOOST_CHECK(!stack.empty());
	stack.pop();
	BOOST_CHECK(stack.empty());
}


BOOST_AUTO_TEST_SUITE_END()
