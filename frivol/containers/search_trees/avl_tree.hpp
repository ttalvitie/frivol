#ifndef FRIVOL_CONTAINERS_SEARCH_TREES_AVL_TREE_HPP
#define FRIVOL_CONTAINERS_SEARCH_TREES_AVL_TREE_HPP

#include <frivol/common.hpp>

namespace frivol {
namespace containers {
namespace search_trees {

/// Implementation of SearchTreeConcept using AVL tree. TODO: implement balacing
template <typename ElementT>
class AVLTree {
public:
	struct Iterator;
	
	bool empty() const;
	
	Iterator begin();
	Iterator end();
	
	template <typename FuncT>
	Iterator search(FuncT func);
	
	void erase(Iterator iter);
	Iterator insert(Iterator iter, const ElementT& element);
	
private:
	
};

}
}
}

#include "avl_tree_impl.hpp"

#endif
