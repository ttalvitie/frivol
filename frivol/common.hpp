#ifndef FRIVOL_COMMON_HPP
#define FRIVOL_COMMON_HPP

#include <cstddef>

namespace frivol {

/// Integer type used in container sizes and indices.
typedef std::size_t Idx;

/// Idx value so large that it will be invalid as index.
constexpr std::size_t nil_idx = (std::size_t)-1;

}

#endif
