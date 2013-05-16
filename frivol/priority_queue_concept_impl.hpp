#ifndef FRIVOL_PRIORITY_QUEUE_CONCEPT_IMPL_HPP
#define FRIVOL_PRIORITY_QUEUE_CONCEPT_IMPL_HPP

namespace frivol {

template <typename PriorityT>
DummyPriorityQueue<PriorityT>::DummyPriorityQueue(Idx size)
	: priorities_(size)
{ }

template <typename PriorityT>
std::pair<Idx, PriorityT> DummyPriorityQueue<PriorityT>::pop() {
	Idx best = nil_idx;
	for(Idx key = 0; key < priorities_.getSize(); ++key) {
		if(priorities_[key].get_ptr() == nullptr) continue;
		
		if(best == nil_idx || priorities_[key].get() < priorities_[best].get()) {
			best = key;
		}
	}
	
	PriorityT best_priority = priorities_[best].get();
	priorities_[best].reset();
	return std::make_pair(best, best_priority);
}

template <typename PriorityT>
bool DummyPriorityQueue<PriorityT>::empty() const {
	for(Idx i = 0; i < priorities_.getSize(); ++i) {
		if(priorities_[i].get_ptr() != nullptr) return false;
	}
	return true;
}

template <typename PriorityT>
void DummyPriorityQueue<PriorityT>::setPriority(Idx key, PriorityT priority) {
	priorities_[key] = priority;
}

template <typename PriorityT>
void DummyPriorityQueue<PriorityT>::setPriorityNIL(Idx key) {
	priorities_[key].reset();
}

}

#endif
