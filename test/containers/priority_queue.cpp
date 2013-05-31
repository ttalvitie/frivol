#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <frivol/containers/priority_queue_concept.hpp>

#include <frivol/containers/priority_queues/dummy_priority_queue.hpp>
#include <frivol/containers/priority_queues/binary_heap.hpp>

using namespace frivol;
using namespace frivol::containers;
using namespace frivol::containers::priority_queues;

BOOST_AUTO_TEST_SUITE(priority_queue)

typedef boost::mpl::list<DummyPriorityQueue<double>, BinaryHeap<double>> PriorityQueueTypes;

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


BOOST_AUTO_TEST_CASE_TEMPLATE(basic, PriorityQueue, PriorityQueueTypes) {
	PriorityQueue q(15);
	q.setPriority(2, 5.55);
	q.setPriority(5, -351);
	q.setPriority(7, 7);
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(5, -351)));
	q.setPriority(1, 2);
	q.setPriority(13, 4);
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(1, 2)));
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(13, 4)));
	q.setPriorityNIL(2);
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(7, 7)));
	BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bigger_basic, PriorityQueue, PriorityQueueTypes) {
	PriorityQueue q(15);
	q.setPriority(0, 3);
	q.setPriority(1, 2);
	q.setPriority(2, 0);
	q.setPriority(3, 1);
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(2, 0)));
	q.setPriority(5, 0);
	q.setPriority(4, 8);
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(5, 0)));
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(3, 1)));
	q.setPriority(14, 6);
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(1, 2)));
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(0, 3)));
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(14, 6)));
	BOOST_CHECK(q.pop() == (std::pair<Idx, double>(4, 8)));
	BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(big_sort, PriorityQueue, PriorityQueueTypes) {
	const Idx n = 20;
	double numbers[n] = {38, 76, 26, 100, 90, 52, 100, 65, 45, 65, 46, 78, 41, 63, 100, 46, 28, 23, 80, 95};
	double sorted[n] = {23, 26, 28, 38, 41, 45, 46, 46, 52, 63, 65, 65, 76, 78, 80, 90, 95, 100, 100, 100};
	
	PriorityQueue q(n);
	for(Idx i = 0; i < n; ++i) {
		q.setPriority(i, numbers[i]);
	}
	for(Idx i = 0; i < n; ++i) {
		BOOST_CHECK(q.pop().second == sorted[i]);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(binary_heap_removal_hard_case, PriorityQueue, PriorityQueueTypes) {
	PriorityQueue q(17);
	q.setPriority(0, 0);
	q.setPriority(10, 10);
	q.setPriority(1, 1);
	q.setPriority(11, 11);
	q.setPriority(12, 12);
	q.setPriority(2, 2);
	q.setPriority(5, 5);
	q.setPriority(13, 13);
	q.setPriority(14, 14);
	q.setPriority(15, 15);
	q.setPriority(16, 16);
	q.setPriority(4, 4);
	q.setPriority(3, 3);
	q.setPriorityNIL(13);
	BOOST_CHECK_EQUAL(q.pop().first, 0);
	BOOST_CHECK_EQUAL(q.pop().first, 1);
	BOOST_CHECK_EQUAL(q.pop().first, 2);
	BOOST_CHECK_EQUAL(q.pop().first, 3);
	BOOST_CHECK_EQUAL(q.pop().first, 4);
	BOOST_CHECK_EQUAL(q.pop().first, 5);
	BOOST_CHECK_EQUAL(q.pop().first, 10);
	BOOST_CHECK_EQUAL(q.pop().first, 11);
	BOOST_CHECK_EQUAL(q.pop().first, 12);
	BOOST_CHECK_EQUAL(q.pop().first, 14);
	BOOST_CHECK_EQUAL(q.pop().first, 15);
	BOOST_CHECK_EQUAL(q.pop().first, 16);
	BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
