#ifndef FRIVOL_FORTUNE_IMPL_HPP
#define FRIVOL_FORTUNE_IMPL_HPP

namespace frivol {
namespace fortune {

template <typename PolicyT>
Algorithm<PolicyT>::Algorithm(const Array<PointT>& sites)
	: n_(sites.getSize()),
	  sites_(sites),
	  event_queue_(3 * n_ - 1),
	  arc_iterators_by_id_(2 * n_ - 1),
	  voronoi_vertex_count_(0)
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

template <typename PolicyT>
void Algorithm<PolicyT>::step() {
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

template <typename PolicyT>
typename PolicyT::Coord Algorithm<PolicyT>::getY() const {
	return y_;
}

template <typename PolicyT>
bool Algorithm<PolicyT>::isFinished() {
	return event_queue_.empty();
}

template <typename PolicyT>
void Algorithm<PolicyT>::finish() {
	while(!isFinished()) step();
}

template <typename PolicyT>
int Algorithm<PolicyT>::getVoronoiVertexCount() const {
	return voronoi_vertex_count_;
}

template <typename PolicyT>
Idx Algorithm<PolicyT>::insertArc_(BeachLineIteratorT pos, Idx site) {
	assert(!free_arc_ids_.empty());
	Idx arc_id = free_arc_ids_.top();
	free_arc_ids_.pop();
	
	BeachLineIteratorT iter = beach_line_.insert(pos, Arc{site, arc_id});
	arc_iterators_by_id_[arc_id] = iter;
	
	return arc_id;
}

template <typename PolicyT>
void Algorithm<PolicyT>::removeArc_(Idx arc_id) {
	// Remove the arc from the beach line.
	beach_line_.erase(arc_iterators_by_id_[arc_id]);
	
	// Return the arc_id back to stack of free IDs.
	free_arc_ids_.push(arc_id);
}


template <typename PolicyT>
Idx Algorithm<PolicyT>::getCircleEventKey_(Idx arc_id) {
	return n_ + arc_id;
}
	
template <typename PolicyT>
Idx Algorithm<PolicyT>::getSiteEventKey_(Idx site) {
	return site;
}

template <typename PolicyT>
std::pair<bool, Idx> Algorithm<PolicyT>::getEventInfo_(Idx event_key) {
	if(event_key < n_) {
		return std::make_pair(true, event_key);
	} else {
		return std::make_pair(false, event_key - n_);
	}
}

template <typename PolicyT>
void Algorithm<PolicyT>::addCircleEvent_(Idx arc_id) {
	BeachLineIteratorT middle = arc_iterators_by_id_[arc_id];
	assert(middle->arc_id == arc_id);
	
	// If the arc is the leftmost or the rightmost, there can't be circle events.
	if(middle == beach_line_.begin()) return;
	BeachLineIteratorT left = middle;
	--left;
	
	BeachLineIteratorT right = middle;
	++right;
	if(right == beach_line_.end()) return;
	
	const PointT& left_point = sites_[left->site];
	const PointT& middle_point = sites_[middle->site];
	const PointT& right_point = sites_[right->site];
	
	// The arcs converge if the sites form a convex triangle.
	if(!GeometryTraitsT::isCCW(left_point, middle_point, right_point)) return;
	
	// The event happens in the top tangent of the circumscribed circle.
	CoordT event_y = GeometryTraitsT::getCircumcircleTopY(
		left_point, middle_point, right_point
	);
	
	// Make sure that our y is monotonous.
	event_y = std::max(event_y, y_);
	
	event_queue_.setPriority(getCircleEventKey_(arc_id), event_y);
}

template <typename PolicyT>
void Algorithm<PolicyT>::handleSiteEvent_(Idx site) {
	// TODO: split function into parts.
	
	// If the beach line is empty, the new arc is the only arc.
	if(beach_line_.empty()) {
		insertArc_(beach_line_.end(), site);
		return;
	}
	
	// Otherwise, search for the right arc on which to place the new arc.
	BeachLineIteratorT base = beach_line_.search([&](BeachLineIteratorT iter) {
		if(iter != beach_line_.begin()) {
			BeachLineIteratorT left = iter;
			--left;
			
			CoordT breakpoint_x =
				GeometryTraitsT::getBreakpointX(sites_[left->site], sites_[iter->site], y_);
			if(sites_[site].x < breakpoint_x) {
				return -1;
			}
		}
		
		BeachLineIteratorT right = iter;
		++right;
		if(right != beach_line_.end()) {
			CoordT breakpoint_x =
				GeometryTraitsT::getBreakpointX(sites_[iter->site], sites_[right->site], y_);
			if(sites_[site].x > breakpoint_x) {
				return 1;
			}
		}
		
		return 0;
	});
	
	assert(base != beach_line_.end());
	
	// The possible circle event around base is false alarm.
	event_queue_.setPriorityNIL(getCircleEventKey_(base->arc_id));
	
	// Add the new arc in the middle of arc in base.
	Idx left_arc_id = insertArc_(base, base->site);
	insertArc_(base, site);
	
	// Add the possible new circle events.
	addCircleEvent_(left_arc_id);
	addCircleEvent_(base->arc_id);
}

template <typename PolicyT>
void Algorithm<PolicyT>::handleCircleEvent_(Idx arc_id) {
	BeachLineIteratorT middle = arc_iterators_by_id_[arc_id];
	BeachLineIteratorT left = middle;
	--left;
	BeachLineIteratorT right = middle;
	++right;
	
	// Each circle event means a new voronoi vertex.
	++voronoi_vertex_count_;
	
	// The possible circle events at left and right are false alarms because
	// we are removing one converging arc.
	event_queue_.setPriorityNIL(getCircleEventKey_(left->arc_id));
	event_queue_.setPriorityNIL(getCircleEventKey_(right->arc_id));
	
	// Remove the middle arc because the left and right arcs run over it.
	removeArc_(middle->arc_id);
	
	// Add the possible new circle events;
	addCircleEvent_(left->arc_id);
	addCircleEvent_(right->arc_id);
}

}
}

#endif
