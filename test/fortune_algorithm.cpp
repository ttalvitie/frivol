#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include "frivol/fortune.hpp"

using namespace frivol;

BOOST_AUTO_TEST_SUITE(fortune_algorithm)

BOOST_AUTO_TEST_CASE(two_horizontal_sites_zero_voronoi_vertices) {
	Array<Point<>> sites(2);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(1, 0);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(0, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(two_vertical_sites_zero_voronoi_vertices) {
	Array<Point<>> sites(2);
	sites[0] = Point<>(4, 3);
	sites[1] = Point<>(4, 4);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(0, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_CASE(sites_in_triangle_one_voronoi_vertex) {
	Array<Point<>> sites(3);
	sites[0] = Point<>(0, 0);
	sites[1] = Point<>(2, 0);
	sites[2] = Point<>(1, 1);
	fortune::Algorithm<> algo(sites);
	algo.finish();
	BOOST_CHECK_EQUAL(1, algo.getVoronoiVertexCount());
}

BOOST_AUTO_TEST_SUITE_END()
