namespace frivol {
namespace fortune {

template <typename PolicyT>
BeachLine<PolicyT>::BeachLine(const containers::Array<PointT>& sites, Idx max_arcs)
	: sites_(sites),
	  max_arcs_(max_arcs),
	  arc_iterators_by_id_(max_arcs)
{
	// Initially, all arc IDs are free.
	for(Idx arc_id = 0; arc_id < max_arcs_; ++arc_id) {
		free_arc_ids_.push(arc_id);
	}
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::getMaxArcCount() const {
	return max_arcs_;
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::insertArc(Idx site, const CoordT& sweepline_y) {
	// Search for an arc on which to place the new arc.
	const CoordT& x = sites_[site].x;
	
	auto order = [this, &x, &sweepline_y](SearchTreeIteratorT iter) {
		return this->orderArcX_(x, iter->arc_id, sweepline_y);
	};
	
	SearchTreeIteratorT base_iter = beach_line_.search(order);
	
	// If there was no match, this is the only arc. Otherwise, split the
	// base arc to two parts and place the new arc in between.
	if(base_iter == beach_line_.end()) {
		return insertArcTo_(beach_line_.end(), site);
	} else {
		Idx left_arc_id = insertArcTo_(base_iter, base_iter->site);
		Idx new_arc_id;
		try {
			new_arc_id = insertArcTo_(base_iter, site);
		} catch(std::logic_error& e) {
			// In case of exception we have to free the left arc so that this
			// function call won't change anything.
			removeArc(left_arc_id);
			
			throw;
		}
		
		return new_arc_id;
	}
}

template <typename PolicyT>
void BeachLine<PolicyT>::removeArc(Idx arc_id) {
	SearchTreeIteratorT iter = arc_iterators_by_id_[arc_id];
	beach_line_.erase(iter);
	free_arc_ids_.push(arc_id);
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::getLeftArc(Idx arc_id) {
	SearchTreeIteratorT iter = arc_iterators_by_id_[arc_id];
	
	if(iter == beach_line_.begin()) {
		return nil_idx;
	} else {
		--iter;
		return iter->arc_id;
	}
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::getRightArc(Idx arc_id) {
	SearchTreeIteratorT iter = arc_iterators_by_id_[arc_id];
	++iter;
	
	if(iter == beach_line_.end()) {
		return nil_idx;
	} else {
		return iter->arc_id;
	}
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::getLeftmostArc() {
	if(beach_line_.empty()) return nil_idx;
	return beach_line_.begin()->arc_id;
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::getRightmostArc() {
	if(beach_line_.empty()) return nil_idx;
	SearchTreeIteratorT iter = beach_line_.end();
	--iter;
	return iter->arc_id;
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::getOriginSite(Idx arc_id) {
	SearchTreeIteratorT iter = arc_iterators_by_id_[arc_id];
	return iter->site;
}

template <typename PolicyT>
Idx BeachLine<PolicyT>::insertArcTo_(SearchTreeIteratorT base_iter, Idx site) {
	if(free_arc_ids_.empty()) {
		throw std::logic_error("BeachLine::insertArc: maximum number of arcs exceeded.");
	}
	
	Idx arc_id = free_arc_ids_.top();
	free_arc_ids_.pop();
	
	Arc arc = {site, arc_id};
	SearchTreeIteratorT iter = beach_line_.insert(base_iter, arc);
	
	arc_iterators_by_id_[arc_id] = iter;
	
	return arc_id;
}

template <typename PolicyT>
int BeachLine<PolicyT>::orderArcX_(
	const CoordT& x,
	Idx arc_id,
	const CoordT& sweepline_y
) {
	Idx left_arc_id = getLeftArc(arc_id);
	Idx right_arc_id = getRightArc(arc_id);
	
	Idx site = getOriginSite(arc_id);
	
	if(left_arc_id != nil_idx) {
		Idx left_site = getOriginSite(left_arc_id);
		
		CoordT breakpoint_x = GeometryTraitsT::getBreakpointX(
			sites_[left_site], sites_[site], sweepline_y, false
		);
		
		if(x < breakpoint_x) return -1;
	}
	
	if(right_arc_id != nil_idx) {
		Idx right_site = getOriginSite(right_arc_id);
		
		CoordT breakpoint_x = GeometryTraitsT::getBreakpointX(
			sites_[site], sites_[right_site], sweepline_y, true
		);
		
		if(x > breakpoint_x) return 1;
	}
	
	return 0;
}

}
}
