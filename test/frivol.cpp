#include <boost/test/unit_test.hpp>

#include <frivol/frivol.hpp>
#include <random>

using namespace frivol;

BOOST_AUTO_TEST_SUITE(frivol)

double distance2(const Point<>& a, const Point<>& b) {
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	return std::sqrt(dx * dx + dy * dy);
}

double distance(const Point<>& a, const Point<>& b) {
	return std::sqrt(distance2(a, b));
}

// Test case that generates random set of points and checks that the
// voronoi diagram is right in random points.
BOOST_AUTO_TEST_CASE(compute_voronoi_diagram_works) {
	const int site_count = 500;
	const int query_count = 5000;
	
	std::mt19937 rng;
	std::uniform_real_distribution<double> site_dist(0, 1);
	
	containers::Array<Point<>> sites(site_count);
	for(int sitei = 0; sitei < site_count; ++sitei) {
		sites[sitei] = Point<>(site_dist(rng), site_dist(rng));
	}
	
	VoronoiDiagram<> vd = computeVoronoiDiagram(sites);
	
	std::uniform_real_distribution<double> query_dist(-0.2, 1.2);
	for(int queryi = 0; queryi < query_count; ++queryi) {
		Point<> query_point(query_dist(rng), query_dist(rng));
		
		// Find the closest site to query_point.
		int closest_site = -1;
		double closest_dist = std::numeric_limits<double>::infinity();
		bool too_close = false;
		for(int sitei = 0; sitei < site_count; ++sitei) {
			double dist = distance(query_point, sites[sitei]);
			
			if(dist < closest_dist) {
				if(closest_dist - dist < 1e-5) {
					too_close = true;
					break;
				}
				closest_dist = dist;
				closest_site = sitei;
			}
		}
		// If the query point was too close to boundary, try again.
		if(too_close) {
			--queryi;
			continue;
		}
		
		// Check which faces of the Voronoi diagram the point is inside.
		// It should be only the face of closest_site.
		for(int sitei = 0; sitei < site_count; ++sitei) {
			bool inside = true;
			// Point is inside convex polygon if it is inside from each edge.
			int start_edge = vd.getFaceBoundaryEdge(sitei);
			double dist2_site = distance2(sites[sitei], query_point);
			int edge = start_edge;
			do {
				int other_site = vd.getIncidentFace(vd.getTwinEdge(edge));
				if(dist2_site > distance2(sites[other_site], query_point)) {
					inside = false;
				}
				
				edge = vd.getNextEdge(edge);
			} while(edge != start_edge);
			
			BOOST_CHECK_EQUAL(inside, sitei == closest_site);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
