#ifndef FRIVOL_POLICY_HPP
#define FRIVOL_POLICY_HPP

#include "priority_queue_concept.hpp"
#include "search_tree_concept.hpp"

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
	template <typename PriorityT> class EventQueueT,
	template <typename ElementT> class BeachLineT
>
struct Policy {
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<CoordT>));
	
	typedef CoordT Coord;
	
	template <typename PriorityT>
	using EventQueue = EventQueueT<PriorityT>;
	
	template <typename ElementT>
	using BeachLine = BeachLineT<ElementT>;
};

/// The default policy using double as coordinate type and the (currently) best
/// data structures.
typedef Policy<double, DummyPriorityQueue, DummySearchTree> DefaultPolicy;

}

#endif
