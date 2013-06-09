#ifndef FRIVOL_VORONOI_DIAGRAM_HPP
#define FRIVOL_VORONOI_DIAGRAM_HPP

#include <frivol/containers/array.hpp>
#include <frivol/containers/dynamic_array.hpp>
#include <frivol/point.hpp>

namespace frivol {

/// Structure for storing a Voronoi diagram. The diagram consists of faces
/// (one for each input site), edges between faces and Voronoi vertices
/// (the endpoints of edges).
/// 
/// One vertex is always incident to three edges - vertices with four or more
/// incident edges can be achieved by using duplicate voronoi vertices and
/// edges of length 0. Each edge is actually a pair half-edges (twin edges)
/// that represent the sides of the edge. The half-edges are thought to be
/// directed so that they cycle around a face counterclockwise.
/// 
/// The faces, half-edges and Voronoi vertices are identified by numerical IDs
/// 0...count-1. The ID of the faces should be the same as their corresponding
/// input site indices.
/// @tparam CoordT Coordinate type of the points stored in the Voronoi diagram.
template <typename CoordT = double>
class VoronoiDiagram {
public:
	typedef Point<CoordT> PointT;
	
	/// Constructs Voronoi diagram.
	/// @param faces Number of faces.
	VoronoiDiagram(Idx faces);
	
	/// Returns the number of faces in the diagram.
	Idx getFaceCount() const;
	
	/// Returns the number of half-edges in the diagram.
	Idx getEdgeCount() const;
	
	/// Returns the number of Voronoi vertices in the diagram.
	Idx getVertexCount() const;
	
	/// Returns the ID of a half-edge that is on the boundary of given face.
	/// If the diagram is incomplete and no such edges have been found or there
	/// is only one face, nil_idx is returned.
	/// @param face ID of the face that the half-edge should be incident to.
	Idx getFaceBoundaryEdge(Idx face) const;
	
	/// Returns the ID of the twin half-edge of given half-edge. Twin
	/// half-edges are the sides of an edge, having opposite directions.
	/// @param edge ID of the half-edge.
	Idx getTwinEdge(Idx edge) const;
	
	/// Returns the ID of the incident face of given half-edge.
	/// @param edge ID of the half-edge.
	Idx getIncidentFace(Idx edge) const;
	
	/// Returns the ID of the Voronoi vertex in the start of given half-edge.
	/// If the diagram is incomplete and the vertex has not yet been found,
	/// nil_idx is returned.
	/// @param edge ID of the half-edge.
	Idx getStartVertex(Idx edge) const;
	
	/// Returns the ID of the Voronoi vertex in the end of given half-edge.
	/// If the diagram is incomplete and the vertex has not yet been found,
	/// nil_idx is returned.
	/// @param edge ID of the half-edge.
	Idx getEndVertex(Idx edge) const;
	
	/// Returns the ID of the next half-edge around the incident face.
	/// If the diagram is incomplete and the next edge has not yet been found,
	/// nil_idx is returned.
	/// @param edge ID of the half-edge.
	Idx getNextEdge(Idx edge) const;
	
	/// Returns the ID of the previous half-edge around the incident face.
	/// If the diagram is incomplete and the previous edge has not yet been found,
	/// nil_idx is returned.
	/// @param edge ID of the half-edge.
	Idx getPreviousEdge(Idx edge) const;
	
	/// Returns the position of a Voronoi vertex.
	/// @param vertex ID of the Voronoi vertex.
	const PointT& getVertexPosition(Idx vertex) const;
	

	/// Adds a new edge (two twin half-edges) to the Voronoi diagram.
	/// @param face1,face2 The IDs of the faces incident to the edge.
	/// @returns the IDs of the new half-edges, first one having face1 and
	/// the second one having face2 as incident face.
	std::pair<Idx, Idx> addEdge(Idx face1, Idx face2);
	
	/// Adds a new Voronoi vertex.
	/// @param pos Position of the vertex.
	/// @param edge1,edge2,edge3 The half-edges having the new vertex as end
	/// vertex, in counterclockwise order.
	/// @returns the ID of the new vertex.
	Idx addVertex(const PointT& pos, Idx edge1, Idx edge2, Idx edge3);
	
	/// Mark half-edges as being consecutive. Done automatically by addVertex
	/// for non-infinite ends of edges.
	/// @param edge1,edge2 The IDs of the half-edges such that edge2 should be
	/// next from edge1.
	void consecutiveEdges(Idx edge1, Idx edge2);
	
private:
	/// Data stored for each half-edge of the Voronoi diagram. If a member has
	/// not yet been populated, nil_idx is stored.
	struct Edge {
		/// Index of the vertex in which the half-edge ends. Has value nil_idx
		/// if the half-edge ends in infinity.
		Idx end_vertex;
		
		/// Index of the incident face.
		Idx face;
		
		/// The next half-edge (from end_vertex) around 'face'. Can be
		/// disconnected from this half-edge if this half-edge ends in infinity.
		Idx next_edge;
		
		/// The previous half-edge (to start_vertex) around 'face'. Can be
		/// disconnected from this half-edge if this half-edge starts in infinity.
		Idx prev_edge;
	};
	
	
	/// Index of one boundary edge for each face. If no edges has been found for
	/// a site, nil_idx is stored.
	containers::Array<Idx> face_boundary_edge_;
	
	/// Information for each half-edge. The twin half-edges should always be in
	/// pairs, so that 2i and 2i+1 are twins for all i.
	containers::DynamicArray<Edge> edges_;
	
	/// The positions of the Voronoi vertices.
	containers::DynamicArray<PointT> vertex_pos_;
};

}

#include "voronoi_diagram_impl.hpp"

#endif


