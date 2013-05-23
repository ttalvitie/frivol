#ifndef FRIVOL_BEACH_LINE_HPP
#define FRIVOL_BEACH_LINE_HPP

#include <frivol/containers/search_tree_concept.hpp>
#include <frivol/containers/stack.hpp>
#include <frivol/geometry_traits.hpp>

#include <stdexcept>

namespace frivol {
namespace fortune {

/// Information of an arc in BeachLine.
struct Arc {
	Idx site;   ///< The index of the site from which the arc originates.
	Idx arc_id; ///< The ID of the arc.
};

/// The advancing sweepline of Fortune's algorithm. Consists of parabolic arcs
/// that are defined to be the curves that have equal distances from an input
/// site and from the sweepline. Adjacent arcs are separated by their
/// intersection points called breakpoints.
/// 
/// The arcs of the beach line are identified by numerical arc IDs. The maximum
/// number of arcs in the beach line must be specified in advance.
/// 
/// @tparam PolicyT The algorithm policy to use, instance of Policy template.
template <typename PolicyT>
class BeachLine {
public:
	typedef typename PolicyT::Coord CoordT;
	typedef Point<CoordT> PointT;
	
	/// Constructs BeachLine.
	/// @param sites The input sites for the algorithm.
	/// @param max_arcs The number of arcs the beach line must be able to
	/// contain.
	BeachLine(const containers::Array<PointT>& sites, Idx max_arcs);
	
	/// Gets the maximum number of arcs there can be in the beach line. The arc
	/// IDs are in 0, ..., getMaxArcCount()-1.
	Idx getMaxArcCount() const;
	
	/// Adds arc to the right place in the beach line. The arc ending up under
	/// the new arc is split in two.
	/// @param site The origin site of the arc.
	/// @param sweepline_y The Y-coordinate of the sweep line that defines the
	/// parabolas.
	/// @returns pair consisting of the ID of the new arc and the ID of the arc
	/// on which it was placed and that was split in two (the returned part being
	/// on the right of the new arc), or nil_idx if the beach line was empty.
	/// @throws std::logic_error if the maximum number of arcs (getMaxArcCount())
	/// are already in use.
	std::pair<Idx, Idx> insertArc(Idx site, const CoordT& sweepline_y);
	
	/// Removes arc from the beach line.
	/// @param arc_id The ID of the arc to remove.
	void removeArc(Idx arc_id);
	
	/// Returns the ID of the arc left from given arc.
	/// @param arc_id ID of the arc.
	/// @returns arc ID of the arc to the left from arc_id, or nil_idx if arc_id
	/// is the leftmost arc.
	Idx getLeftArc(Idx arc_id) const;
	
	/// Returns the ID of the arc right from given arc.
	/// @param arc_id ID of the arc.
	/// @returns arc ID of the arc to the right from arc_id, or nil_idx if arc_id
	/// is the rightmost arc.
	Idx getRightArc(Idx arc_id) const;
	
	/// Returns the index of the origin site of given arc.
	/// @param arc_id ID of the arc.
	Idx getOriginSite(Idx arc_id) const;
	
private:
	typedef typename PolicyT::template BeachLineSearchTree<Arc> SearchTreeT;
	typedef typename SearchTreeT::Iterator SearchTreeIteratorT;
	BOOST_CONCEPT_ASSERT((containers::SearchTreeConcept<SearchTreeT, Arc>));
	
	typedef GeometryTraits<CoordT> GeometryTraitsT;
	
	/// Adds new arc to given position in the beach line.
	/// @param base_iter The iterator of the position before which to place the new arc.
	/// @param site The origin site of the new arc.
	/// @returns The arc ID of the new arc.
	/// @throws std::logic_error if the maximum number of arcs (getMaxArcCount())
	/// are already in use.
	Idx insertArcTo_(SearchTreeIteratorT base_iter, Idx site);
	
	/// Returns -1, 0 or 1 if given X-coordinate is left from, in the middle of
	/// or right from given arc.
	/// @param x The X-coordinate to search for.
	/// @param arc_id The ID of the arc.
	/// @param sweepline_y The Y-coordinate of the sweep line that defines the
	/// parabolas.
	int orderArcX_(const CoordT& x, Idx arc_id, const CoordT& sweepline_y) const;
	
	
	/// Array of input point sites.
	const containers::Array<PointT>& sites_;
	
	/// The arcs of the beach line, ordered by X-coordinate.
	SearchTreeT beach_line_;
	
	/// The maximum number of arcs on the beach line.
	Idx max_arcs_;
	
	/// Stack of currently unoccupied arc IDs from 0, ..., max_arcs-1.
	containers::Stack<Idx> free_arc_ids_;
	
	/// Mapping from beach line arc IDs to their corresponding iterators
	/// in beach_line_.
	containers::Array<SearchTreeIteratorT> arc_iterators_by_id_;
};

}
}

#include "beach_line_impl.hpp"

#endif
