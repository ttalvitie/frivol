#ifndef FRIVOL_FORTUNE_FORTUNE_HPP
#define FRIVOL_FORTUNE_FORTUNE_HPP

#include <frivol/containers/priority_queue_concept.hpp>
#include <frivol/fortune/beach_line.hpp>
#include <frivol/policy.hpp>
#include <frivol/voronoi_diagram.hpp>
#include <frivol/geometry_traits.hpp>

namespace frivol {
namespace fortune {

/// State of Fortune's algorithm.
/// @tparam PolicyT The algorithm policy to use, instance of Policy template.
template <typename PolicyT = DefaultPolicy>
class Algorithm {
public:
	typedef typename PolicyT::Coord CoordT;
	typedef Point<CoordT> PointT;
	typedef VoronoiDiagram<CoordT> VoronoiDiagramT;
	
	/// Constructs algorithm state.
	/// @param points Reference to the input set of sites. The object must
	/// exist throughout the existence of the Algorithm.
	Algorithm(const containers::Array<PointT>& sites);
	
	/// Runs the algorithm one event handling forward.
	void step();
	
	/// Returns the sweepline Y coordinate of last step(). Undefined return
	/// value if step() has not been called yet.
	CoordT getSweeplineY() const;
	
	/// Returns true if the algorithm has finished.
	bool isFinished();
	
	/// Steps the algorithm until the end.
	void finish();
	
	/// Returns the number of Voronoi vertices met in the algorithm.
	int getVoronoiVertexCount() const;
	
private:
	typedef BeachLine<PolicyT> BeachLineT;
	
	typedef typename PolicyT::template EventPriorityQueue<CoordT> EventPriorityQueueT;
	BOOST_CONCEPT_ASSERT((containers::PriorityQueueConcept<EventPriorityQueueT, CoordT>));
	
	typedef GeometryTraits<CoordT> GeometryTraitsT;
	
	
	/// Returns the event queue key for circle event of given arc.
	/// @param arc_id The id of the arc.
	Idx getCircleEventKey_(Idx arc_id) const;
	
	/// Get the event queue key for site event.
	/// @param site The index of the site.
	Idx getSiteEventKey_(Idx site) const;
	
	/// Get the number of possible event keys. The event keys are in range
	/// 0, ..., getEventKeyCount_()-1.
	Idx getEventKeyCount_() const;
	
	/// Returns pair of:
	///  - boolean with value true if the event is a site event, false if
	///    the event is a circle event.
	///  - In case of site event, the index of the site. In case of circle
	///    event, the arc ID.
	/// @param event_key The key of the event.
	std::pair<bool, Idx> getEventInfo_(Idx event_key) const;
	
	/// Check whether the breakpoints around an arc converge, and if they do,
	/// add a circle event to the queue.
	/// @param arc_id The ID of the arc.
	void tryAddCircleEvent_(Idx arc_id);
	
	/// Handles site event.
	/// @param site The index of the site.
	void handleSiteEvent_(Idx site);
	
	/// Handles circle event.
	/// @param arc_id The ID of the disappearing arc.
	void handleCircleEvent_(Idx arc_id);
	
	
	/// The input set of point sites.
	const containers::Array<PointT>& sites_;
	
	/// The beach line of arcs.
	BeachLineT beach_line_;
	
	/// The y coordinate of last step. Undefined value if step has not been
	/// called.
	CoordT sweepline_y_;
	
	/// The event queue of site events and circle events. The event keys
	/// can be translated with #getCircleEventKey_, #getSiteEventKey_ and
	/// #getEventInfo_.
	EventPriorityQueueT event_queue_;
	
	/// The output Voronoi diagram that is constructed by the algorithm.
	VoronoiDiagramT diagram_;
	
	/// Indexes of the half-edges the breakpoints are drawing, indexed by the
	/// arc IDs of the arcs left from the breakpoints.
	containers::Array<Idx> breakpoint_edge_index_;
};

}
}

#include "fortune_impl.hpp"

#endif
