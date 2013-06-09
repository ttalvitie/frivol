#include <frivol/frivol.hpp>

// Object used in emscripten C interface containing input sites and the diagram.
struct Voronoi {
	Voronoi() : diagram(0) { }
	
	frivol::containers::Array<frivol::Point<>> sites; // Array of the sites.
	frivol::VoronoiDiagram<> diagram; // The voronoi diagram.
};

// Get endpoint of infinite edge between site1 and site2.
std::pair<double, double> getInfinitePoint(
	const frivol::containers::Array<frivol::Point<>>& sites,
	frivol::Idx site1,
	frivol::Idx site2
) {
	double midx = 0.5 * (sites[site1].x + sites[site2].x);
	double midy = 0.5 * (sites[site1].y + sites[site2].y);
	double dx = sites[site2].x - sites[site1].x;
	double dy = sites[site2].y - sites[site1].y;
	double coef;
	if(std::abs(dx) > std::abs(dy)) {
		coef = 1e5 / std::abs(dx);
	} else {
		coef = 1e5 / std::abs(dy);
	}
	return std::make_pair(midx - coef * dy, midy + coef * dx);
}

// C interface usable from emscripten.
extern "C" {

// Compute a Voronoi diagram of 'site_count' sites from array 'sites' and
// (each point occupies two indices, first one is X coordinate and the second Y).
// The returned pointer must be freed by frivoldraw_FreeDiagram.
Voronoi* frivoldraw_ComputeVoronoi(double* sites, int site_count) {
	Voronoi* voronoi = new Voronoi();
	voronoi->sites.resize(site_count);
	double* sites_pos = sites;
	for(int sitei = 0; sitei < site_count; ++sitei) {
		voronoi->sites[sitei].x = *(sites_pos++);
		voronoi->sites[sitei].y = *(sites_pos++);
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
	return voronoi->diagram.getEdgeCount();
}

// Get the endpoint X- or Y-coordinates of Voronoi half-edge of index 'edge'.
// If the end is infinite, X- and Y-coordinates will be large.
double frivoldraw_GetStartX(Voronoi* voronoi, int edge) {
	frivol::Idx vertex = voronoi->diagram.getStartVertex(edge);
	if(vertex == frivol::nil_idx) {
		return getInfinitePoint(
			voronoi->sites,
			voronoi->diagram.getIncidentFace(voronoi->diagram.getTwinEdge(edge)),
			voronoi->diagram.getIncidentFace(edge)
		).first;
	} else {
		return voronoi->diagram.getVertexPosition(vertex).x;
	}
}
double frivoldraw_GetStartY(Voronoi* voronoi, int edge) {
	frivol::Idx vertex = voronoi->diagram.getStartVertex(edge);
	if(vertex == frivol::nil_idx) {
		return getInfinitePoint(
			voronoi->sites,
			voronoi->diagram.getIncidentFace(voronoi->diagram.getTwinEdge(edge)),
			voronoi->diagram.getIncidentFace(edge)
		).second;
	} else {
		return voronoi->diagram.getVertexPosition(vertex).y;
	}
}
double frivoldraw_GetEndX(Voronoi* voronoi, int edge) {
	return frivoldraw_GetStartX(voronoi, voronoi->diagram.getTwinEdge(edge));
}
double frivoldraw_GetEndY(Voronoi* voronoi, int edge) {
	return frivoldraw_GetStartY(voronoi, voronoi->diagram.getTwinEdge(edge));
}

}
