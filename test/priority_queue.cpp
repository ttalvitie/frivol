#include <boost/test/unit_test.hpp>

#include "frivol/priority_queue_concept.hpp"

using namespace frivol;

BOOST_AUTO_TEST_SUITE(priority_queue)

typedef boost::mpl::list<DummyPriorityQueue<double>> PriorityQueueTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(set_and_empty_works, PriorityQueue, PriorityQueueTypes) {
	PriorityQueue q(5);
	BOOST_CHECK(q.empty());
	q.setPriority(3, 3.14);
	BOOST_CHECK(!q.empty());
	q.setPriorityNIL(3);
	BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pop_works, PriorityQueue, PriorityQueueTypes) {
	PriorityQueue q(7);
	q.setPriority(2, 5.55);
	q.setPriority(5, -351);
	BOOST_CHECK_EQUAL(q.pop(), 5);
	BOOST_CHECK(!q.empty());
	BOOST_CHECK_EQUAL(q.pop(), 2);
	BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
