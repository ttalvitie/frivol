namespace frivol {
namespace fortune {

template <typename PolicyT>
Algorithm<PolicyT>::Algorithm(const containers::Array<PointT>& sites)
	: sites_(sites),
	  // 2 * sites.getSize() - 1 arcs will suffice because it is the theoretical limit
	  // of arcs on the beach line concurrently.
	  beach_line_(sites_, 2 * sites.getSize() - 1),
	  event_queue_(getEventKeyCount_()),
	  voronoi_vertex_count_(0)
{
	// All sites must have events in the queue.
	for(Idx i = 0; i < sites_.getSize(); ++i) {
		event_queue_.setPriority(getSiteEventKey_(i), sites_[i].y);
	}
	
	// Initialize one face for each input site, initially having no incident
	// edge.
	for(Idx i = 0; i < sites_.getSize(); ++i) {
		diagram_.faces.add(typename VoronoiDiagramT::Face{nil_idx});
	}
}

template <typename PolicyT>
void Algorithm<PolicyT>::step() {
	if(event_queue_.empty()) return;
	
	Idx event_key;
	std::tie(event_key, sweepline_y_) = event_queue_.pop();
	
	std::pair<bool, Idx> event_info = getEventInfo_(event_key);
	bool is_site_event = event_info.first;
	
	if(is_site_event) {
		handleSiteEvent_(event_info.second);
	} else {
		handleCircleEvent_(event_info.second);
	}
}

template <typename PolicyT>
typename PolicyT::Coord Algorithm<PolicyT>::getSweeplineY() const {
	return sweepline_y_;
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
Idx Algorithm<PolicyT>::getCircleEventKey_(Idx arc_id) const {
	// The arc IDs 0, ..., sites_.getSize()-1 are reserved for site events,
	// so we need to shift circle events by sites_.getSize().
	return sites_.getSize() + arc_id;
}

template <typename PolicyT>
Idx Algorithm<PolicyT>::getSiteEventKey_(Idx site) const {
	return site;
}

template <typename PolicyT>
Idx Algorithm<PolicyT>::getEventKeyCount_() const {
	// The possible events are:
	//  - site events: sites_.getSize()
	//  - circle events: beach_line_.getMaxArcCount().
	return sites_.getSize() + beach_line_.getMaxArcCount();
}

template <typename PolicyT>
std::pair<bool, Idx> Algorithm<PolicyT>::getEventInfo_(Idx event_key) const {
	if(event_key < sites_.getSize()) {
		return std::make_pair(true, event_key);
	} else {
		return std::make_pair(false, event_key - sites_.getSize());
	}
}

template <typename PolicyT>
void Algorithm<PolicyT>::tryAddCircleEvent_(Idx arc_id) {
	// If the arc is the leftmost or the rightmost, there can't be circle events.
	Idx left_arc_id = beach_line_.getLeftArc(arc_id);
	Idx right_arc_id = beach_line_.getRightArc(arc_id);
	if(left_arc_id == nil_idx || right_arc_id == nil_idx) return;
	
	const PointT& left_point = sites_[beach_line_.getOriginSite(left_arc_id)];
	const PointT& middle_point = sites_[beach_line_.getOriginSite(arc_id)];
	const PointT& right_point = sites_[beach_line_.getOriginSite(right_arc_id)];
	
	// The arcs converge if the sites form a convex triangle.
	if(!GeometryTraitsT::isCCW(left_point, middle_point, right_point)) return;
	
	// The event happens in the top tangent of the circumscribed circle.
	CoordT event_y = GeometryTraitsT::getCircumcircleTopY(
		left_point, middle_point, right_point
	);
	
	// Make sure that our y is monotonous.
	event_y = std::max(event_y, sweepline_y_);
	
	event_queue_.setPriority(getCircleEventKey_(arc_id), event_y);
}

template <typename PolicyT>
void Algorithm<PolicyT>::handleSiteEvent_(Idx site) {
	Idx arc_id = beach_line_.insertArc(site, sweepline_y_);
	
	Idx left_arc_id = beach_line_.getLeftArc(arc_id);
	Idx right_arc_id = beach_line_.getRightArc(arc_id);
	
	// If there are arcs around the new arc, the beach line was not empty.
	if(right_arc_id != nil_idx) {
		// The possible circle event around the arc over which the new arc
		// was placed was false alarm because the situation around it has changed.
		event_queue_.setPriorityNIL(getCircleEventKey_(right_arc_id));
		
		// Add the possible new circle events.
		tryAddCircleEvent_(left_arc_id);
		tryAddCircleEvent_(right_arc_id);
	}
}

template <typename PolicyT>
void Algorithm<PolicyT>::handleCircleEvent_(Idx arc_id) {
	// Each circle event means a new voronoi vertex.
	++voronoi_vertex_count_;
	
	// The possible circle event around the left and right arcs are false alarms
	// because the situations in them have changed.
	Idx left_arc_id = beach_line_.getLeftArc(arc_id);
	Idx right_arc_id = beach_line_.getRightArc(arc_id);
	event_queue_.setPriorityNIL(getCircleEventKey_(left_arc_id));
	event_queue_.setPriorityNIL(getCircleEventKey_(right_arc_id));
	
	beach_line_.removeArc(arc_id);
	
	// Add the possible new circle events;
	tryAddCircleEvent_(left_arc_id);
	tryAddCircleEvent_(right_arc_id);
}

}
}
