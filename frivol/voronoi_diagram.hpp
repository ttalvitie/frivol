#ifndef FRIVOL_VORONOI_DIAGRAM_HPP
#define FRIVOL_VORONOI_DIAGRAM_HPP

#include <frivol/containers/dynamic_array.hpp>
#include <frivol/point.hpp>

namespace frivol {

/// Structure for storing a Voronoi diagram. The diagram consists of faces
/// (one for each input site), edges between faces and Voronoi vertices
/// (the endpoints of edges).
/// 
/// One vertex is always incident to three edges - vertices with four or more
/// incident edges can be achieved by using duplicate voronoi vertices and
/// edges of length 0. Each edge is actually a pair half-edges that represent
/// the sides of the edge. The half-edges are thought to be directed so that
/// they cycle around a face counterclockwise.
/// 
/// The vertices faces, half-edges and Voronoi vertices are identified by their
/// numerical indices in the member arrays.
template <typename CoordT>
struct VoronoiDiagram {
	/// Data stored for each face of the Voronoi diagram.
	struct Face {
		/// Index of one of the half-edges incident to the face.
		Idx incident_edge;
	};
	
	/// Data stored for each half-edge of the Voronoi diagram.
	struct Edge {
		/// Index of the vertex in which the half-edge ends. Has value nil_idx
		/// if the half-edge ends in infinity.
		Idx end_vertex;
		
		/// Index of the other side of the same edge, to the opposite direction.
		Idx twin_edge;
		
		/// Index of the incident face.
		Idx face;
		
		/// The previous half-edge (to start_vertex) around 'face'. Can be
		/// disconnected from this half-edge if this half-edge starts in infinity.
		Idx prev_edge;
		
		/// The next half-edge (from end_vertex) around 'face'. Can be
		/// disconnected from this half-edge if this half-edge ends in infinity.
		Idx next_edge;
	};
	
	/// Data stored for each vertex of the Voronoi diagram.
	struct Vertex {
		/// Indices of the half-edges from the vertex, in counterclockwise order.
		Idx edges[3];
		
		/// The position of the vertex.
		Point<CoordT> position;
	};
	
	
	/// The faces of the Voronoi diagram. Should have the same indexing as their
	/// corresponding input sites.
	containers::DynamicArray<Face> faces;
	
	/// The half-edges of the Voronoi diagram.
	containers::DynamicArray<Edge> edges;
	
	/// The voronoi vertices the Voronoi diagram.
	containers::DynamicArray<Vertex> vertices;
};


}

#endif


