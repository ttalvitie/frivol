#ifndef FRIVOL_FRIVOL_HPP
#define FRIVOL_FRIVOL_HPP

#include "policy.hpp"
#include "array.hpp"
#include "point.hpp"

namespace frivol {

/// Computes the Voronoi diagram of a set of point sites.
/// @tparam PolicyT The algorithm policy to use, instance of Policy template.
/// @param sites Array containing all the sites.
/// @todo Currently unfinished, therefore prints debugging information and
/// doesn't return anything.
template <typename PolicyT = DefaultPolicy>
void computeVoronoiDiagram(const Array<Point<typename PolicyT::Coord>>& sites);

}

#include "frivol_impl.hpp"

#endif
