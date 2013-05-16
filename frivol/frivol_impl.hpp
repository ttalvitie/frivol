#ifndef FRIVOL_FRIVOL_IMPL_HPP
#define FRIVOL_FRIVOL_IMPL_HPP

#include "priority_queue_concept.hpp"
#include "search_tree_concept.hpp"
#include "stack.hpp"

#include <cassert>
#include <cmath>

namespace frivol {

/// Classes for the implementation of Fortune's algorithm.
namespace fortune_ {

/// Information of an arc in the beach line.
struct Arc {
	Idx site; ///< The site from which the arc originates.
	Idx id;   ///< The ID of the arc.
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
	Algorithm(const Array<PointT>& sites)
		: n_(sites.getSize()),
		  sites_(sites),
		  event_queue_(3 * n_ - 1),
		  arc_iterators_by_id_(2 * n_ - 1)
	{
		// Populate free arc IDs.
		for(int i = 0; i < 2 * n_ - 1; ++i) {
			free_arc_ids_.push(i);
		}
		
		// Add site events to the event queue.
		for(Idx i = 0; i < n_; ++i) {
			event_queue_.setPriority(getSiteEventKey_(i), sites_[i].y);
		}
	}
	
	/// Run the algorithm one event handling forward.
	void step() {
		if(event_queue_.empty()) return;
		
		Idx event_key;
		std::tie(event_key, y_) = event_queue_.pop();
		
		std::pair<bool, Idx> event_info = getEventInfo_(event_key);
		bool is_site_event = event_info.first;
		
		if(is_site_event) {
			handleSiteEvent_(event_info.second);
		} else {
			handleCircleEvent_(event_info.second);
		}
	}
	
	/// Get the y coordinate of last step(). Undefined return value if step()
	/// has not been called yet.
	CoordT getY() {
		return y_;
	}
	
	/// Returns true if the algorithm has finished.
	bool isFinished() {
		return event_queue_.empty();
	}
	
	/// Steps the algorithm until the end and returns the result.
	/// @todo Currently unfinished, therefore doesn't return anything.
	void finish() {
		while(!isFinished()) step();
	}
	
private:
	typedef typename PolicyT::template BeachLine<Arc> BeachLineT;
	typedef typename BeachLineT::Iterator BeachLineIteratorT;
	BOOST_CONCEPT_ASSERT((SearchTreeConcept<BeachLineT, Arc>));
	
	typedef typename PolicyT::template EventQueue<CoordT> EventQueueT;
	BOOST_CONCEPT_ASSERT((PriorityQueueConcept<EventQueueT, CoordT>));
	
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
	
	/// Get the event queue key for circle event of given arc.
	/// @param arc_id The id of the arc.
	Idx getCircleEventKey_(Idx arc_id) {
		return n_ + arc_id;
	}
	
	/// Get the event queue key for site event.
	/// @param site The index of the site.
	Idx getSiteEventKey_(Idx site) {
		return site;
	}
	
	/// Returns pair of:
	///  - boolean with value true if the event is a site event, false if
	///    the event is a circle event.
	///  - In case of site event, the index of the site. In case of circle
	///    event, the arc ID.
	/// @param event_key The key of the event.
	std::pair<bool, Idx> getEventInfo_(Idx event_key) {
		if(event_key < n_) {
			return std::make_pair(true, event_key);
		} else {
			return std::make_pair(false, event_key - n_);
		}
	}
	
	/// Handle site event.
	/// @param site The index of the site.
	void handleSiteEvent_(Idx site) {
		// TODO: implement
/*
		// There should be available arc IDs.
		assert(!free_arc_ids_.empty());
		
		// Allocate an arc id for the new arc.
		Idx arc_id = free_arc_ids_.top();
		free_arc_ids_.pop();
		
		// Search for the right arc on which to place the new ar.
		BeachLineItertorT base = search([&](BeachLineItertorT iter) {
			if(iter != beach_line_.begin()) {
				BeachLineItertorT left = iter;
				--left;
				
			}
		});
*/
	}
	
	/// Handle circle event.
	/// @param arc_id The ID of the arc.
	void handleCircleEvent_(Idx circle) {
		// TODO: implement
	}
};

}

template <typename PolicyT = DefaultPolicy>
void computeVoronoiDiagram(const Array<Point<typename PolicyT::Coord>>& sites) {
	using namespace fortune_;
	
	Algorithm<PolicyT> algorithm(sites);
	algorithm.finish();
}

}

#include "frivol_impl.hpp"

#endif
