#ifndef FRIVOL_FORTUNE_HPP
#define FRIVOL_FORTUNE_HPP

#include "priority_queue_concept.hpp"
#include "search_tree_concept.hpp"
#include "geometry_traits.hpp"
#include "stack.hpp"

namespace frivol {
namespace fortune {

/// Information of an arc in the beach line.
struct Arc {
	Idx site;   ///< The site from which the arc originates.
	Idx arc_id; ///< The ID of the arc.
};

/// State of Fortune's algorithm.
/// @tparam PolicyT The algorithm policy to use, instance of Policy template.
template <typename PolicyT>
class Algorithm {
public:
	typedef typename PolicyT::Coord CoordT;
	typedef Point<CoordT> PointT;
	
	/// Constructs algorithm state.
	/// @param points Reference to the input set of sites. The object must
	/// exist throughout the existence of the Algorithm.
	Algorithm(const Array<PointT>& sites);
	
	/// Run the algorithm one event handling forward.
	void step();
	
	/// Get the y coordinate of last step(). Undefined return value if step()
	/// has not been called yet.
	CoordT getY();
	
	/// Returns true if the algorithm has finished.
	bool isFinished();
	
	/// Steps the algorithm until the end and returns the result.
	/// @todo Currently unfinished, therefore doesn't return anything.
	void finish();
	
private:
	typedef typename PolicyT::template BeachLine<Arc> BeachLineT;
	typedef typename BeachLineT::Iterator BeachLineIteratorT;
	BOOST_CONCEPT_ASSERT((SearchTreeConcept<BeachLineT, Arc>));
	
	typedef typename PolicyT::template EventQueue<CoordT> EventQueueT;
	BOOST_CONCEPT_ASSERT((PriorityQueueConcept<EventQueueT, CoordT>));
	
	typedef GeometryTraits<CoordT> GeometryTraitsT;
	
	
	/// Get the event queue key for circle event of given arc.
	/// @param arc_id The id of the arc.
	Idx getCircleEventKey_(Idx arc_id);
	
	/// Get the event queue key for site event.
	/// @param site The index of the site.
	Idx getSiteEventKey_(Idx site);
	
	/// Returns pair of:
	///  - boolean with value true if the event is a site event, false if
	///    the event is a circle event.
	///  - In case of site event, the index of the site. In case of circle
	///    event, the arc ID.
	/// @param event_key The key of the event.
	std::pair<bool, Idx> getEventInfo_(Idx event_key);
	
	/// Handle site event.
	/// @param site The index of the site.
	void handleSiteEvent_(Idx site);
	
	/// Handle circle event.
	/// @param arc_id The ID of the arc.
	void handleCircleEvent_(Idx circle);
	
	
	Idx n_;                      ///< Number of sites.
	const Array<PointT>& sites_; ///< The input set of point sites.
	
	/// The y coordinate of last step. Undefined value if step has not been
	/// called.
	CoordT y_;
	
	/// The beach line of arcs ordered by X.
	BeachLineT beach_line_;
	
	/// The event queue of site events and circle events.
	/// @see getCircleEventKey, getSiteEventKey, getEventInfo
	EventQueueT event_queue_;
	
	/// Mapping from beach line arc IDs to their corresponding iterators
	/// in beach_line_. Every arc in the beach line has an unique ID in
	/// 0..2n-2, for use with events_. The free IDs are stored in free_arc_ids_.
	Array<BeachLineIteratorT> arc_iterators_by_id_;
	
	/// Currently free beach line IDs in 0..2n-2. Guaranteed to contain enough
	/// IDs because it is known that beach line consists of at most 2n-1 arcs.
	Stack<Idx> free_arc_ids_;
};

}
}

#include "fortune_impl.hpp"

#endif
