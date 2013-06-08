namespace frivol {
namespace fortune {

template <typename PolicyT>
Algorithm<PolicyT>::Algorithm(const containers::Array<PointT>& sites)
	: sites_(sites),
	  beach_line_(sites_, std::max(2 * sites.getSize(), (Idx)1) - 1),
	  event_queue_(getEventKeyCount_()),
	  diagram_(sites_.getSize()),
	  breakpoint_edge_index_(beach_line_.getMaxArcCount())
{
	// All sites must have events in the queue.
	for(Idx i = 0; i < sites_.getSize(); ++i) {
		event_queue_.setPriority(getSiteEventKey_(i), sites_[i].y);
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
	
	// If we are done now, mark all infinite edges consecutive in the Voronoi
	// diagram.
	if(event_queue_.empty()) markConsecutiveInfiniteEdges_();
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
	return diagram_.getVertexCount();
}

template <typename PolicyT>
const VoronoiDiagram<typename PolicyT::Coord>& 
Algorithm<PolicyT>::getVoronoiDiagram() const {
	return diagram_;
}

template <typename PolicyT>
VoronoiDiagram<typename PolicyT::Coord> Algorithm<PolicyT>::extractVoronoiDiagram(
	Algorithm<PolicyT>&& algorithm
) {
	return std::move(algorithm.diagram_);
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
		
		// Add the edge between the sites of the new arc and the arc below it.
		Idx base_site = beach_line_.getOriginSite(right_arc_id);
		
		Idx left_edge, right_edge;
		std::tie(left_edge, right_edge) = diagram_.addEdge(base_site, site);
		
		// Mark the edges to the breakpoints drawing them.
		breakpoint_edge_index_[left_arc_id] = left_edge;
		breakpoint_edge_index_[arc_id] = right_edge;
	}
}

template <typename PolicyT>
void Algorithm<PolicyT>::handleCircleEvent_(Idx arc_id) {
	Idx left_arc_id = beach_line_.getLeftArc(arc_id);
	Idx right_arc_id = beach_line_.getRightArc(arc_id);
	
	Idx left_site = beach_line_.getOriginSite(left_arc_id);
	Idx site = beach_line_.getOriginSite(arc_id);
	Idx right_site = beach_line_.getOriginSite(right_arc_id);
	
	// Add the new Voronoi vertex.
	PointT vertex_pos = GeometryTraitsT::getCircumcenter(
		sites_[left_site], sites_[site], sites_[right_site]
	);
	
	Idx left_edge = breakpoint_edge_index_[left_arc_id];
	Idx right_edge = breakpoint_edge_index_[arc_id];
	Idx new_edge_in, new_edge_out;
	std::tie(new_edge_out, new_edge_in) = diagram_.addEdge(left_site, right_site);
	diagram_.addVertex(vertex_pos, new_edge_in, left_edge, right_edge);
	
	// Update the remaining breakpoint to draw the right edge.
	breakpoint_edge_index_[left_arc_id] = new_edge_out;
	
	// The possible circle event around the left and right arcs are false alarms
	// because the situations in them have changed.
	event_queue_.setPriorityNIL(getCircleEventKey_(left_arc_id));
	event_queue_.setPriorityNIL(getCircleEventKey_(right_arc_id));
	
	beach_line_.removeArc(arc_id);
	
	// Add the possible new circle events;
	tryAddCircleEvent_(left_arc_id);
	tryAddCircleEvent_(right_arc_id);
}

template <typename PolicyT>
void Algorithm<PolicyT>::markConsecutiveInfiniteEdges_() {
	// If there are only zero or one arcs, there's nothing to do.
	if(beach_line_.getLeftmostArc() == nil_idx) return;
	if(beach_line_.getLeftmostArc() == beach_line_.getRightmostArc()) return;
	
	// Cycle through consecutive pairs of breakpoints.
	Idx arc1 = beach_line_.getLeftArc(beach_line_.getRightmostArc());
	for(
		Idx arc2 = beach_line_.getLeftmostArc();
		arc2 != beach_line_.getRightmostArc();
		arc2 = beach_line_.getRightArc(arc2)
	) {
		diagram_.consecutiveEdges(
			breakpoint_edge_index_[arc2],
			diagram_.getTwinEdge(breakpoint_edge_index_[arc1])
		);
		
		arc1 = arc2;
	}
}

}
}
