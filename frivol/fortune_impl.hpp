#ifndef FRIVOL_FORTUNE_IMPL_HPP
#define FRIVOL_FORTUNE_IMPL_HPP

namespace frivol {
namespace fortune {

template <typename PolicyT>
Algorithm<PolicyT>::Algorithm(const Array<PointT>& sites)
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
typename PolicyT::Coord Algorithm<PolicyT>::getY() {
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
void Algorithm<PolicyT>::handleSiteEvent_(Idx site) {
	// Allocate an arc id for the new arc.
	assert(!free_arc_ids_.empty());
	Idx arc_id = free_arc_ids_.top();
	free_arc_ids_.pop();
	
	// If the beach line is empty, the new arc is the only arc.
	if(beach_line_.empty()) {
		beach_line_.insert(beach_line_.end(), Arc{site, arc_id});
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
	assert(!free_arc_ids_.empty());
	Idx left_arc_id = free_arc_ids_.top();
	free_arc_ids_.pop();
	beach_line_.insert(base, Arc{base->site, left_arc_id});
	beach_line_.insert(base, Arc{site, arc_id});
	
	// TODO: add circle events.
}

template <typename PolicyT>
void Algorithm<PolicyT>::handleCircleEvent_(Idx circle) {
	// TODO: implement
}

}
}

#include "frivol_impl.hpp"

#endif
