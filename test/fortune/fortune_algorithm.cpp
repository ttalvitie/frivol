#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <cmath>

#include <frivol/fortune/fortune.hpp>

using namespace frivol;

BOOST_AUTO_TEST_SUITE(fortune_algorithm)

BOOST_AUTO_TEST_CASE(two_horizontal_sites_zero_voronoi_vertices) {
	containers::Array<Point<>> sites(2);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(1, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(0, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(two_vertical_sites_zero_voronoi_vertices) {
	containers::Array<Point<>> sites(2);
	sites[0] = Point<>(4, 3);
	sites[1] = Point<>(4, 4);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(0, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(sites_in_triangle_one_voronoi_vertex) {
	containers::Array<Point<>> sites(3);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(2, 0);
	sites[2] = Point<>(1, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(1, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(diamond_two_voronoi_vertices) {
	containers::Array<Point<>> sites(4);
	sites[0] = Point<>(-2, 0);
	sites[1] = Point<>(2, 0);
	sites[2] = Point<>(0, -1);
	sites[3] = Point<>(0, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(2, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(four_sites_three_voronoi_vertices) {
	containers::Array<Point<>> sites(4);
	sites[0] = Point<>(-1, 0);
	sites[1] = Point<>(1, 0);
	sites[2] = Point<>(0, 1);
	sites[3] = Point<>(0, 2);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(3, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(hourglass_four_voronoi_vertices) {
	containers::Array<Point<>> sites(5);
	sites[0] = Point<>(-1, -2);
	sites[1] = Point<>(1, -2);
	sites[2] = Point<>(-1, 2);
	sites[3] = Point<>(1, 2);
	sites[4] = Point<>(0, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(4, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(n_gon_n_voronoi_vertices) {
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
	BOOST_CHECK_EQUAL(n, algo.getVoronoiVertexCount());
}

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
	sites[0] = Point<>(1, 0);
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

BOOST_AUTO_TEST_SUITE_END()
