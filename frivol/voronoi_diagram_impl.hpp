namespace frivol {

template <typename CoordT>
VoronoiDiagram<CoordT>::VoronoiDiagram(Idx faces)
	: face_boundary_edge_(faces)
{
	for(Idx i = 0; i < faces; ++i) {
		face_boundary_edge_[i] = nil_idx;
	}
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getFaceCount() const {
	return face_boundary_edge_.getSize();
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getEdgeCount() const {
	return edges_.getSize();
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getVertexCount() const {
	return vertex_pos_.getSize();
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getFaceBoundaryEdge(Idx face) const {
	return face_boundary_edge_[face];
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getTwinEdge(Idx edge) const {
	// Flipping first bit adds one to even numbers and subtracts one from odd
	// numbers.
	return edge ^ 1;
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getIncidentFace(Idx edge) const {
	return edges_[edge].face;
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getStartVertex(Idx edge) const {
	return edges_[getTwinEdge(edge)].end_vertex;
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getEndVertex(Idx edge) const {
	return edges_[edge].end_vertex;
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getNextEdge(Idx edge) const {
	return edges_[edge].next_edge;
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::getPreviousEdge(Idx edge) const {
	return edges_[edge].prev_edge;
}

template <typename CoordT>
const Point<CoordT>& VoronoiDiagram<CoordT>::getVertexPosition(Idx vertex) const {
	return vertex_pos_[vertex];
}

template <typename CoordT>
std::pair<Idx, Idx> VoronoiDiagram<CoordT>::addEdge(Idx face1, Idx face2) {
	Edge edge1, edge2;
	
	edge1.face = face1;
	edge2.face = face2;
	
	edge1.end_vertex = nil_idx;
	edge2.end_vertex = nil_idx;
	edge1.next_edge = nil_idx;
	edge2.next_edge = nil_idx;
	edge1.prev_edge = nil_idx;
	edge2.prev_edge = nil_idx;
	
	Idx id1 = edges_.add(edge1);
	Idx id2 = edges_.add(edge2);
	
	if(face_boundary_edge_[face1] == nil_idx) {
		face_boundary_edge_[face1] = id1;
	}
	if(face_boundary_edge_[face2] == nil_idx) {
		face_boundary_edge_[face2] = id2;
	}
	
	return std::make_pair(id1, id2);
}

template <typename CoordT>
Idx VoronoiDiagram<CoordT>::addVertex(
	const PointT& pos, Idx edge1, Idx edge2, Idx edge3
) {
	Idx vertex = vertex_pos_.add(pos);
	
	edges_[edge1].end_vertex = vertex;
	edges_[edge2].end_vertex = vertex;
	edges_[edge3].end_vertex = vertex;
	
	consecutiveEdges(edge1, getTwinEdge(edge3));
	consecutiveEdges(edge2, getTwinEdge(edge1));
	consecutiveEdges(edge3, getTwinEdge(edge2));
	
	return vertex;
}

template <typename CoordT>
void VoronoiDiagram<CoordT>::consecutiveEdges(Idx edge1, Idx edge2) {
	edges_[edge1].next_edge = edge2;
	edges_[edge2].prev_edge = edge1;
}

}
