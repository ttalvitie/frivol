#ifndef FRIVOL_PRIORITY_QUEUE_CONCEPT_HPP
#define FRIVOL_PRIORITY_QUEUE_CONCEPT_HPP

#include "common.hpp"
#include "array.hpp"

#include <boost/concept_check.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>

namespace frivol {

/// Concept checking class for priority queues X with priority values of type
/// PriorityT (or NIL). Priority queues are initialized with given size, and
/// contain priority values for keys 0, 1, ..., size-1. Initially, all priority
/// values are NIL. C must support the following operations:
///  - <construct>(Idx size) creates priority queue for keys 0, 1, ..., size-1.
///  - bool empty() const returns true if all keys have NIL priority.
///  - Idx pop() returns the key with lowest non-NIL priority and sets the
///    priority of that key to NIL.
///  - void setPriority(Idx key, PriorityT priority) sets the priority value of
///    'key' to non-NIL value 'priority'.
///  - void setPriorityNIL(Idx key) sets the priority value of key 'key' to NIL.
/// X may assume that PriorityT is ordered with <-operator. X may have
/// undefined behavior if supplied keys are out of range or if pop() is called
/// when empty() returns true.
template <typename X, typename PriorityT>
class PriorityQueueConcept {
public:
	BOOST_CONCEPT_ASSERT((boost::LessThanComparable<PriorityT>));
	BOOST_CONCEPT_USAGE(PriorityQueueConcept) {
		X x(size);
		x.setPriorityNIL(key);
		x.setPriority(key, priority);
		sameType(x.empty(), bool());
		sameType(x.pop(), key);
	}
	
	Idx size;
	Idx key;
	PriorityT priority;
	
private:
	/// Function that causes compile time error if called with parameters of
	/// different types.
	template <typename T>
	void sameType(const T&, const T&);
};

/// Simple implementation of PriorityQueueConcept.
template <typename PriorityT>
class DummyPriorityQueue {
public:
	BOOST_CONCEPT_ASSERT((boost::LessThanComparable<PriorityT>));
	
	DummyPriorityQueue(Idx size);
	
	Idx pop();
	
	bool empty() const;
	
	void setPriority(Idx key, PriorityT priority);
	void setPriorityNIL(Idx key);
	
private:
	typedef boost::optional<PriorityT> OptionalPriorityT;
	
	/// Array of priority values (empty in case of NIL).
	Array<OptionalPriorityT> priorities_;
};

}

#include "priority_queue_concept_impl.hpp"

#endif