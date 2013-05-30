#include <boost/test/unit_test.hpp>

#include <frivol/voronoi_diagram.hpp>

using namespace frivol;

BOOST_AUTO_TEST_SUITE(voronoi_diagram)

BOOST_AUTO_TEST_CASE(accessors_work_by_default) {
	VoronoiDiagram<double> vd(3);
	BOOST_CHECK_EQUAL(vd.getFaceCount(), 3);
	BOOST_CHECK_EQUAL(vd.getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(vd.getVertexCount(), 0);
	
	BOOST_CHECK_EQUAL(vd.getFaceBoundaryEdge(0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getFaceBoundaryEdge(1), nil_idx);
	BOOST_CHECK_EQUAL(vd.getFaceBoundaryEdge(2), nil_idx);
}

BOOST_AUTO_TEST_CASE(edge_addition_works) {
	VoronoiDiagram<double> vd(2);
	
	Idx edge0, edge1;
	std::tie(edge0, edge1) = vd.addEdge(0, 1);
	
	BOOST_CHECK_EQUAL(vd.getTwinEdge(edge0), edge1);
	BOOST_CHECK_EQUAL(vd.getTwinEdge(edge1), edge0);
	
	BOOST_CHECK_EQUAL(vd.getIncidentFace(edge0), 0);
	BOOST_CHECK_EQUAL(vd.getIncidentFace(edge1), 1);
	
	BOOST_CHECK_EQUAL(vd.getFaceBoundaryEdge(0), edge0);
	BOOST_CHECK_EQUAL(vd.getFaceBoundaryEdge(1), edge1);
	
	BOOST_CHECK_EQUAL(vd.getNextEdge(edge0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getPreviousEdge(edge0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getNextEdge(edge1), nil_idx);
	BOOST_CHECK_EQUAL(vd.getPreviousEdge(edge1), nil_idx);
	
	BOOST_CHECK_EQUAL(vd.getStartVertex(edge0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getEndVertex(edge0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getStartVertex(edge1), nil_idx);
	BOOST_CHECK_EQUAL(vd.getEndVertex(edge1), nil_idx);
}

BOOST_AUTO_TEST_CASE(vertex_addition_works) {
	VoronoiDiagram<double> vd(5);
	
	Idx edge0, edge1, edge2;
	edge0 = vd.addEdge(1, 2).first;
	edge1 = vd.addEdge(2, 0).first;
	edge2 = vd.addEdge(0, 1).first;
	vd.addVertex(Point<>(0.5, -341.2), edge0, edge1, edge2);
	
	BOOST_CHECK_EQUAL(vd.getVertexCount(), 1);
	BOOST_CHECK_EQUAL(vd.getEdgeCount(), 6);
	
	BOOST_CHECK_EQUAL(vd.getVertexPosition(0).x, 0.5);
	BOOST_CHECK_EQUAL(vd.getVertexPosition(0).y, -341.2);
	
	// The vertices should be correctly marked
	BOOST_CHECK_EQUAL(vd.getStartVertex(edge0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getEndVertex(edge0), 0);
	BOOST_CHECK_EQUAL(vd.getStartVertex(edge1), nil_idx);
	BOOST_CHECK_EQUAL(vd.getEndVertex(edge1), 0);
	BOOST_CHECK_EQUAL(vd.getStartVertex(edge2), nil_idx);
	BOOST_CHECK_EQUAL(vd.getEndVertex(edge2), 0);
	
	// And the consecutivity of the edges.
	BOOST_CHECK_EQUAL(vd.getPreviousEdge(edge0), nil_idx);
	BOOST_CHECK_EQUAL(vd.getNextEdge(edge0), vd.getTwinEdge(edge2));
	BOOST_CHECK_EQUAL(vd.getPreviousEdge(edge1), nil_idx);
	BOOST_CHECK_EQUAL(vd.getNextEdge(edge1), vd.getTwinEdge(edge0));
	BOOST_CHECK_EQUAL(vd.getPreviousEdge(edge2), nil_idx);
	BOOST_CHECK_EQUAL(vd.getNextEdge(edge2), vd.getTwinEdge(edge1));
}


BOOST_AUTO_TEST_SUITE_END()
