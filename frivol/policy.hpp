#ifndef FRIVOL_POLICY_HPP
#define FRIVOL_POLICY_HPP

#include <frivol/containers/priority_queue_concept.hpp>
#include <frivol/containers/search_tree_concept.hpp>

#include <frivol/containers/priority_queues/binary_heap.hpp>
#include <frivol/containers/search_trees/avl_tree.hpp>

#include <frivol/geometry_traits.hpp>

namespace frivol {

/// Policy class for the Fortune's algorithm, specifying data types and data
/// structures to use.
/// @tparam CoordT The coordinate type to use. Should be ordered and default
/// constructible to undefined value. Should have specialization of
/// GeometryTraits.
/// @tparam EventQueueT The priority queue type for events. Must conform to
/// PriorityQueueConcept.
/// @tparam BeachLineT The search tree to use for the "beach line" of arcs.
/// Must conform to SearchTreeConcept.
template <
	typename CoordT,
	template <typename PriorityT> class EventPriorityQueueT,
	template <typename ElementT> class BeachLineSearchTreeT
>
struct Policy {
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<CoordT>));
	BOOST_CONCEPT_ASSERT((boost::LessThanComparable<CoordT>));
	BOOST_CONCEPT_ASSERT((GeometryTraitsImplementedConcept<CoordT>));
	
	typedef CoordT Coord;
	
	template <typename PriorityT>
	using EventPriorityQueue = EventPriorityQueueT<PriorityT>;
	
	template <typename ElementT>
	using BeachLineSearchTree = BeachLineSearchTreeT<ElementT>;
};

/// The default policy using double as coordinate type and the (currently) best
/// data structures.
typedef Policy<
	double,
	containers::priority_queues::BinaryHeap,
	containers::search_trees::AVLTree
> DefaultPolicy;

}

#endif
