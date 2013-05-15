#ifndef FRIVOL_POLICY_HPP
#define FRIVOL_POLICY_HPP

#include "priority_queue_concept.hpp"
#include "search_tree_concept.hpp"

namespace frivol {

/// Policy class for the Fortune's algorithm, specifying data types and data
/// structures to use.
/// @tparam CoordT The coordinate type to use. Should be a floating point type
/// or similar type that does not over- or underflow easily.
/// @tparam EventQueueT The priority queue type for events. Must conform to
/// PriorityQueueConcept.
/// @tparam BeachLineT The search tree to use for the "beach line" of arcs.
/// Must conform to SearchTreeConcept.
template <
	typename CoordT,
	template <typename PriorityT> class EventQueueT,
	template <typename ElementT> class BeachLineT
>
struct Policy {
	typedef CoordT Coord;
	
	template <typename PriorityT>
	using EventQueue = EventQueueT<PriorityT>;
	
	template <typename ElementT>
	using BeachLine = EventQueueT<ElementT>;
};

/// The default policy using double as coordinate type and the (currently) best
/// data structures.
typedef Policy<double, DummyPriorityQueue, DummySearchTree> DefaultPolicy;

}

#endif
