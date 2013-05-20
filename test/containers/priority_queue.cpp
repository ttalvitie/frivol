#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <frivol/containers/priority_queue_concept.hpp>

#include <frivol/containers/priority_queues/dummy_priority_queue.hpp>

using namespace frivol;
using namespace frivol::containers;
using namespace frivol::containers::priority_queues;

BOOST_AUTO_TEST_SUITE(priority_queue)

typedef boost::mpl::list<DummyPriorityQueue<double>> PriorityQueueTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(implements_concept, PriorityQueue, PriorityQueueTypes) {
	BOOST_CONCEPT_ASSERT((PriorityQueueConcept<PriorityQueue, double>));
}

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
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(5, -351)));
	BOOST_CHECK(!q.empty());
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(2, 5.55)));
	BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
