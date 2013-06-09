#ifndef FRIVOL_FRIVOL_HPP
#define FRIVOL_FRIVOL_HPP

#include <frivol/fortune/algorithm.hpp>
#include <frivol/voronoi_diagram.hpp>
#include <frivol/point.hpp>
#include <frivol/containers/array.hpp>

namespace frivol {

/// Compute the Voronoi diagram of an array of points.
/// @param sites Array of the points.
/// @returns the Voronoi diagram. The face indices are equal to their
/// corresponding input point indices.
/// @tparam PolicyT The algorithm policy to use, instance of Policy template.
template <typename PolicyT = DefaultPolicy>
VoronoiDiagram<typename PolicyT::Coord> computeVoronoiDiagram(
	const containers::Array<Point<typename PolicyT::Coord>>& sites
);

}

#include "frivol_impl.hpp"

#endif
