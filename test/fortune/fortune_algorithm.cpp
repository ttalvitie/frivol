#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <cmath>

#include <frivol/fortune/algorithm.hpp>

using namespace frivol;

BOOST_AUTO_TEST_SUITE(fortune_algorithm)

BOOST_AUTO_TEST_CASE(empty_voronoi_diagram) {
	containers::Array<Point<>> sites;
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 0);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 0);
}

BOOST_AUTO_TEST_CASE(one_site_voronoi_diagram) {
	containers::Array<Point<>> sites(1);
	sites[0] = Point<>(0, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 1);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 0);
	BOOST_CHECK_EQUAL(diagram.getFaceBoundaryEdge(0), nil_idx);
}

BOOST_AUTO_TEST_CASE(two_site_voronoi_diagram) {
	containers::Array<Point<>> sites(2);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(1, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 2);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 2);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 0);
	
	// The edges should be correctly marked incident to the faces.
	BOOST_CHECK(diagram.getFaceBoundaryEdge(0) >= 0);
	BOOST_CHECK(diagram.getFaceBoundaryEdge(0) <= 1);
	BOOST_CHECK(diagram.getFaceBoundaryEdge(1) >= 0);
	BOOST_CHECK(diagram.getFaceBoundaryEdge(1) <= 1);
	BOOST_CHECK(diagram.getFaceBoundaryEdge(0) != diagram.getFaceBoundaryEdge(1));
	
	// Edges should have themselves as previous and next.
	for(int edge = 0; edge < 2; ++edge) {
		BOOST_CHECK_EQUAL(diagram.getNextEdge(edge), edge);
		BOOST_CHECK_EQUAL(diagram.getPreviousEdge(edge), edge);
	}
}

void checkPointsClose(Point<> a, Point<> b) {
	BOOST_CHECK_CLOSE(a.x, b.x, 0.01);
	BOOST_CHECK_CLOSE(a.y, b.y, 0.01);
}

BOOST_AUTO_TEST_CASE(three_site_voronoi_diagram) {
	containers::Array<Point<>> sites(3);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(2, 0);
	sites[2] = Point<>(1, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 3);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 6);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 1);
	
	// Each face should have two incident half-edges
	for(int face = 0; face < 3; ++face) {
		Idx edge1 = diagram.getFaceBoundaryEdge(face);
		
		// Make sure that edge1 is the edge to the vertex.
		if(diagram.getEndVertex(edge1) != 0) edge1 = diagram.getNextEdge(edge1);
		BOOST_CHECK_EQUAL(diagram.getEndVertex(edge1), 0);
		
		Idx edge2 = diagram.getNextEdge(edge1);
		BOOST_CHECK(edge1 != edge2);
		
		// The edges should have other vertices marked correctly too.
		BOOST_CHECK_EQUAL(diagram.getStartVertex(edge1), nil_idx);
		BOOST_CHECK_EQUAL(diagram.getStartVertex(edge2), 0);
		BOOST_CHECK_EQUAL(diagram.getEndVertex(edge2), nil_idx);
		
		// The edges should be previous and next to each other.
		BOOST_CHECK_EQUAL(diagram.getPreviousEdge(edge2), edge1);
		BOOST_CHECK_EQUAL(diagram.getNextEdge(edge2), edge1);
		
		BOOST_CHECK_EQUAL(diagram.getPreviousEdge(edge1), edge2);
		BOOST_CHECK_EQUAL(diagram.getNextEdge(edge1), edge2);
		
		// The edges should have face marked as the incident face.
		BOOST_CHECK_EQUAL(diagram.getIncidentFace(edge1), face);
		BOOST_CHECK_EQUAL(diagram.getIncidentFace(edge2), face);
	}
	
	// Check vertex position.
	checkPointsClose(diagram.getVertexPosition(0), Point<>(1, 0));
}

BOOST_AUTO_TEST_CASE(three_site_voronoi_diagram_flipped) {
	containers::Array<Point<>> sites(3);
	sites[0] = Point<>(2, 0);
	sites[1] = Point<>(0, 0);
	sites[2] = Point<>(1, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 3);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 6);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 1);
}

BOOST_AUTO_TEST_CASE(diamond_voronoi_diagram) {
	containers::Array<Point<>> sites(4);
	sites[0] = Point<>(-2, 0);
	sites[1] = Point<>(2, 0);
	sites[2] = Point<>(0, -1);
	sites[3] = Point<>(0, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 4);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 10);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 2);
	
	Idx left_vertex, right_vertex;
	if(diagram.getVertexPosition(0).x < diagram.getVertexPosition(1).x) {
		left_vertex = 0;
		right_vertex = 1;
	} else {
		left_vertex = 1;
		right_vertex = 0;
	}
	
	checkPointsClose(diagram.getVertexPosition(left_vertex), Point<>(-0.75, 0));
	checkPointsClose(diagram.getVertexPosition(right_vertex), Point<>(0.75, 0));
	
	// Check that each face has the right amount of edges.
	int face_edges[4] = {0};
	for(int edge = 0; edge < 10; ++edge) {
		++face_edges[diagram.getIncidentFace(edge)];
	}
	BOOST_CHECK_EQUAL(face_edges[0], 2);
	BOOST_CHECK_EQUAL(face_edges[1], 2);
	BOOST_CHECK_EQUAL(face_edges[2], 3);
	BOOST_CHECK_EQUAL(face_edges[3], 3);
}

BOOST_AUTO_TEST_CASE(four_sites_three_voronoi_vertices_voronoi_diagram) {
	containers::Array<Point<>> sites(4);
	sites[0] = Point<>(-1, 0);
	sites[1] = Point<>(1, 0);
	sites[2] = Point<>(0, 1);
	sites[3] = Point<>(0, 2);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), 4);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 12);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), 3);
}

BOOST_AUTO_TEST_CASE(n_gon_n_voronoi_diagram) {
	int n = 341;
	containers::Array<Point<>> sites(n + 1);
	double twopi = 8 * std::atan(1.0);
	for(int i = 0; i < n; ++i) {
		double angle = i * twopi / n;
		sites[i] = Point<>(std::cos(angle), std::sin(angle));
	}
	sites[n] = Point<>(0, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	const VoronoiDiagram<double>& diagram = algo.getVoronoiDiagram();
	BOOST_CHECK_EQUAL(diagram.getFaceCount(), n + 1);
	BOOST_CHECK_EQUAL(diagram.getEdgeCount(), 4 * n);
	BOOST_CHECK_EQUAL(diagram.getVertexCount(), n);
	
	// Check that the cycles around faces have right sizes.
	auto checkCycle = [&](Idx start_edge, Idx expected_count) {
		Idx edge = start_edge;
		Idx count = 0;
		do {
			BOOST_CHECK_EQUAL(diagram.getPreviousEdge(diagram.getNextEdge(edge)), edge);
			++count;
			edge = diagram.getNextEdge(edge);
		} while(edge != start_edge);
		BOOST_CHECK_EQUAL(count, expected_count);
	};
	checkCycle(diagram.getFaceBoundaryEdge(0), 3);
	checkCycle(diagram.getFaceBoundaryEdge(3), 3);
	checkCycle(diagram.getFaceBoundaryEdge(n), n);
}

BOOST_AUTO_TEST_CASE(hard_case_three_horizontally_and_one_on_top) {
	containers::Array<Point<>> sites(4);
	sites[0] = Point<>(-1, 0);
	sites[1] = Point<>(0, 0);
	sites[2] = Point<>(1, 0);
	sites[3] = Point<>(0, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(algo.getVoronoiDiagram().getVertexCount(), 2);
}

BOOST_AUTO_TEST_CASE(hard_case_zigzag) {
	containers::Array<Point<>> sites(4);
	sites[0] = Point<>(3, 0);
	sites[1] = Point<>(2, 1);
	sites[2] = Point<>(1, 0);
	sites[3] = Point<>(0, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(algo.getVoronoiDiagram().getVertexCount(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
