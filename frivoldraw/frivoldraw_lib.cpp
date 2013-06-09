#include <frivol/frivol.hpp>

// Object used in emscripten C interface containing input sites and the diagram.
struct Voronoi {
	frivol::containers::Array<frivol::Point<>> sites; // Array of the sites.
	frivol::VoronoiDiagram<> diagram; // The voronoi diagram.
};

// C interface usable from emscripten.
extern "C" {

// Compute a Voronoi diagram of 'point_count' points from array 'points' and
// (each point occupies two indices, first one is X coordinate and the second Y).
// The returned pointer must be freed by frivoldraw_FreeDiagram.
Voronoi* frivoldraw_ComputeVoronoi(double* points, int point_count) {
	Voronoi* voronoi = new Voronoi();
	voronoi->sites.resize(point_count);
	double* points_pos = points;
	for(int pointi = 0; pointi < point_count; ++pointi) {
		voronoi->sites[pointi].x = *(points_pos++);
		voronoi->sites[pointi].y = *(points_pos++);
	}
	voronoi->diagram = std::move(frivol::computeVoronoiDiagram<>(voronoi->sites));
	return voronoi;
}

// Free Voronoi diagram returned by frivoldraw_ComputeDiagram.
void frivoldraw_FreeVoronoi(Voronoi* voronoi) {
	delete voronoi;
}

// Get number of half-edges in the Voronoi diagram.
int frivoldraw_GetEdgeCount(Voronoi* voronoi) {
	return voronoi->diagram->getEdgeCount();
}

// // Get the endpoint X- or Y-coordinates of Voronoi half-edge of index 'edge'.
// // If the end is infinite, X- and Y-coordinates signify the direction.
// int frivoldraw_GetStartX(Voronoi* voronoi, int edge) {
// 	frivol::Idx vertex = voronoi->diagram->getStartVertex(edge);
// 	if(vertex == frivol::nil_idx) {
// 		
// 	} else {
// 		return voronoi->diagram->getVertexPosition(vertex).x;
// 	}
// }

}
