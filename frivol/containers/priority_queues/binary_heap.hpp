#ifndef FRIVOL_CONTAINERS_PRIORITY_QUEUES_BINARY_HEAP_HPP
#define FRIVOL_CONTAINERS_PRIORITY_QUEUES_BINARY_HEAP_HPP

#include <frivol/common.hpp>
#include <frivol/containers/array.hpp>

#include <boost/optional.hpp>

namespace frivol {
namespace containers {
namespace priority_queues {

/// Simple implementation of PriorityQueueConcept.
template <typename PriorityT>
class BinaryHeap {
public:
	BOOST_CONCEPT_ASSERT((boost::LessThanComparable<PriorityT>));
	
	BinaryHeap(Idx size);
	
	std::pair<Idx, PriorityT> pop();
	
	bool empty() const;
	
	void setPriority(Idx key, PriorityT priority);
	void setPriorityNIL(Idx key);
	
private:
	/// Returns the heap index of the parent of given heap index.
	/// @param heap_idx The heap index.
	Idx getHeapParent_(Idx heap_idx) const;
	
	/// Returns the heap index of the left child of given heap index.
	/// @param heap_idx The heap index.
	Idx getHeapLeftChild_(Idx heap_idx) const;
	
	/// Returns the heap index of the right child of given heap index.
	/// @param heap_idx The heap index.
	Idx getHeapRightChild_(Idx heap_idx) const;
	
	/// Removes given element, conserving the heap property.
	/// @param heap_idx The heap index of the element to remove.
	void removeFromHeap_(Idx heap_idx);
	
	/// Swaps two elements in the heap.
	/// @param heap_idx1,heap_idx2 The heap indices of the elements to swap.
	void swapInHeap_(Idx heap_idx1, Idx heap_idx2);
	
	/// Compares the priorities of elements in the heap.
	/// @param heap_idx1,heap_idx2 The heap indices to compare.
	/// @returns true if element at heap_idx1 has higher priority than heap_idx2.
	bool hasHigherPriority_(Idx heap_idx1, Idx heap_idx2);
	
	void checkIntegrity_();
	
	typedef boost::optional<PriorityT> OptionalPriorityT;
	
	/// Array of priority values (empty in case of NIL).
	Array<OptionalPriorityT> priorities_;
	
	/// Size of the heap, i.e. the number of keys with non-NIL priorities.
	Idx heap_size_;
	
	/// The binary heap of the keys is stored in heap_[i], i = 0...non_nil_count_-1.
	Array<Idx> heap_;
	
	/// Indices of the non-NIL elements in the heap by key.
	Array<Idx> heap_indices_;
};

}
}
}

#include "binary_heap_impl.hpp"

#endif
