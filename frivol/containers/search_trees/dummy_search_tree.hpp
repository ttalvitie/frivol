#ifndef FRIVOL_CONTAINERS_SEARCH_TREES_DUMMY_SEARCH_TREE_HPP
#define FRIVOL_CONTAINERS_SEARCH_TREES_DUMMY_SEARCH_TREE_HPP

#include <list>

namespace frivol {
namespace containers {
namespace search_trees {

/// Simple implementation of SearchTreeConcept (a wrapper around std::list).
template <typename ElementT>
class DummySearchTree : private std::list<ElementT> {
public:
	typedef typename std::list<ElementT>::iterator Iterator;
	
	using std::list<ElementT>::empty;
	using std::list<ElementT>::begin;
	using std::list<ElementT>::end;
	using std::list<ElementT>::insert;
	using std::list<ElementT>::erase;
	
	template <typename FuncT>
	Iterator search(FuncT func) {
		BOOST_CONCEPT_ASSERT((boost::UnaryFunction<FuncT, int, Iterator>));
		
		for(Iterator iter = begin(); iter != end(); ++iter) {
			int val = func(iter);
			if(val < 0) break;
			if(val == 0) return iter;
		}
		
		return end();
	}
};

}
}
}

#endif
