#ifndef FRIVOL_FRIVOL_IMPL_HPP
#define FRIVOL_FRIVOL_IMPL_HPP

#include "fortune.hpp"

#include <cassert>
#include <cmath>

namespace frivol {

template <typename PolicyT = DefaultPolicy>
void computeVoronoiDiagram(const Array<Point<typename PolicyT::Coord>>& sites) {
	using namespace fortune;
	
	Algorithm<PolicyT> algorithm(sites);
	algorithm.finish();
}

}

#include "frivol_impl.hpp"

#endif
