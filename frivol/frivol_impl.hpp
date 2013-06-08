namespace frivol {

template <typename PolicyT>
VoronoiDiagram<typename PolicyT::Coord> computeVoronoiDiagram(
	const containers::Array<Point<typename PolicyT::Coord>>& sites
) {
	fortune::Algorithm<PolicyT> algorithm(sites);
	algorithm.finish();
	return fortune::Algorithm<PolicyT>::extractVoronoiDiagram(algorithm);
}

}
