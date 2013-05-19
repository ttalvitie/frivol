#ifndef FRIVOL_CONTAINERS_PRIORITY_QUEUES_DUMMY_PRIORITY_QUEUE_HPP
#define FRIVOL_CONTAINERS_PRIORITY_QUEUES_DUMMY_PRIORITY_QUEUE_HPP

#include <frivol/common.hpp>
#include <frivol/containers/array.hpp>

#include <boost/optional.hpp>

namespace frivol {
namespace containers {
namespace priority_queues {

/// Simple implementation of PriorityQueueConcept.
template <typename PriorityT>
class DummyPriorityQueue {
public:
	BOOST_CONCEPT_ASSERT((boost::LessThanComparable<PriorityT>));
	
	DummyPriorityQueue(Idx size);
	
	std::pair<Idx, PriorityT> pop();
	
	bool empty() const;
	
	void setPriority(Idx key, PriorityT priority);
	void setPriorityNIL(Idx key);
	
private:
	typedef boost::optional<PriorityT> OptionalPriorityT;
	
	/// Array of priority values (empty in case of NIL).
	Array<OptionalPriorityT> priorities_;
};

}
}
}

#include "dummy_priority_queue_impl.hpp"

#endif
