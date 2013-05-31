namespace frivol {
namespace containers {
namespace priority_queues {

template <typename PriorityT>
BinaryHeap<PriorityT>::BinaryHeap(Idx size)
	: priorities_(size),
	  heap_size_(0),
	  heap_(size),
	  heap_indices_(size)
{ }

template <typename PriorityT>
std::pair<Idx, PriorityT> BinaryHeap<PriorityT>::pop() {
	Idx top_key = heap_[0];
	PriorityT top_priority = priorities_[top_key].get();
	priorities_[top_key].reset();
	
	removeFromHeap_(0);
	
	return std::make_pair(top_key, top_priority);
}

template <typename PriorityT>
bool BinaryHeap<PriorityT>::empty() const {
	return heap_size_ == 0;
}

template <typename PriorityT>
void BinaryHeap<PriorityT>::setPriority(Idx key, PriorityT priority) {
	// If the element is in the heap already, remove it so we can add it again.
	setPriorityNIL(key);
	
	priorities_[key] = priority;
	
	Idx heap_idx = heap_size_;
	++heap_size_;
	
	heap_[heap_idx] = key;
	heap_indices_[key] = heap_idx;
	
	bubbleUp_(heap_idx);
}

template <typename PriorityT>
void BinaryHeap<PriorityT>::setPriorityNIL(Idx key) {
	if(priorities_[key].get_ptr() == nullptr) return;
	
	priorities_[key].reset();
	removeFromHeap_(heap_indices_[key]);
}

template <typename PriorityT>
Idx BinaryHeap<PriorityT>::getHeapParent_(Idx heap_idx) const {
	return (heap_idx - 1) / 2;
}

template <typename PriorityT>
Idx BinaryHeap<PriorityT>::getHeapLeftChild_(Idx heap_idx) const {
	return 2 * heap_idx + 1;
}

template <typename PriorityT>
Idx BinaryHeap<PriorityT>::getHeapRightChild_(Idx heap_idx) const {
	return 2 * heap_idx + 2;
}

template <typename PriorityT>
void BinaryHeap<PriorityT>::removeFromHeap_(Idx heap_idx) {
	--heap_size_;
	
	if(heap_idx == heap_size_) return;
	
	heap_[heap_idx] = heap_[heap_size_];
	heap_indices_[heap_[heap_idx]] = heap_idx;
	
	bubbleDown_(heap_idx);
	bubbleUp_(heap_idx);
}

template <typename PriorityT>
void BinaryHeap<PriorityT>::swapInHeap_(Idx heap_idx1, Idx heap_idx2) {
	std::swap(heap_[heap_idx1], heap_[heap_idx2]);
	heap_indices_[heap_[heap_idx1]] = heap_idx1;
	heap_indices_[heap_[heap_idx2]] = heap_idx2;
}

template <typename PriorityT>
bool BinaryHeap<PriorityT>::hasHigherPriority_(Idx heap_idx1, Idx heap_idx2) {
	return priorities_[heap_[heap_idx1]].get() < priorities_[heap_[heap_idx2]].get();
}

template <typename PriorityT>
void BinaryHeap<PriorityT>::bubbleUp_(Idx heap_idx) {
	while(heap_idx != 0) {
		Idx parent = getHeapParent_(heap_idx);
		if(hasHigherPriority_(parent, heap_idx)) break;
		swapInHeap_(heap_idx, parent);
		heap_idx = parent;
	}
}

template <typename PriorityT>
void BinaryHeap<PriorityT>::bubbleDown_(Idx heap_idx) {
	while(getHeapLeftChild_(heap_idx) < heap_size_) {
		Idx left = getHeapLeftChild_(heap_idx);
		Idx right = getHeapRightChild_(heap_idx);
		
		Idx priority_child;
		if(right == heap_size_ || hasHigherPriority_(left, right)) {
			priority_child = left;
		} else {
			priority_child = right;
		}
		
		if(hasHigherPriority_(heap_idx, priority_child)) break;
		
		swapInHeap_(heap_idx, priority_child);
		heap_idx = priority_child;
	}
}

}
}
}
