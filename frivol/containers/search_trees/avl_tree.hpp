#ifndef FRIVOL_CONTAINERS_SEARCH_TREES_AVL_TREE_HPP
#define FRIVOL_CONTAINERS_SEARCH_TREES_AVL_TREE_HPP

#include <frivol/common.hpp>
#include <frivol/containers/search_trees/avl_node.hpp>

namespace frivol {
namespace containers {
namespace search_trees {

// Forward declarations.
template <typename ElementT>
class AVLTree;


/// Standard bidirectional iterator for iterating over the elements of an AVLTree.
/// @tparam ElementT Type of elements stored in the search tree.
template <typename ElementT>
class AVLIterator {
public:
	/// Constructs an invalid iterator.
	AVLIterator() { }

	typedef ElementT value_type;
	typedef ElementT* pointer;
	typedef ElementT& reference;
	typedef ptrdiff_t difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;

	bool operator==(const AVLIterator<ElementT>& other) const;
	bool operator!=(const AVLIterator<ElementT>& other) const;

	ElementT& operator*();
	ElementT* operator->();

	AVLIterator<ElementT>& operator++();
	AVLIterator<ElementT>& operator--();

	AVLIterator<ElementT> operator++(int);
	AVLIterator<ElementT> operator--(int);

private:
	typedef AVLNode<ElementT> Node;
	
	/// Constructs AVL tree iterator.
	/// @param tree Reference to the unique_ptr storing the root node of the tree.
	/// @param node Pointer to the current node, or nullptr for past the end.
	AVLIterator(const std::unique_ptr<Node>& root, Node* node = nullptr);

	/// Pointer to the unique_ptr storing the root node of the tree.
	const std::unique_ptr<Node>* root_;

	/// Pointer to the current node, or nullptr if we are past the end.
	Node* node_;
	
	friend class AVLTree<ElementT>;
};

/// Implementation of SearchTreeConcept using AVL tree.
/// @tparam ElementT Type of elements stored in the search tree.
template <typename ElementT>
class AVLTree {
public:
	typedef AVLIterator<ElementT> Iterator;
	
	AVLTree();
	
	bool empty() const;
	
	Iterator begin();
	Iterator end();
	
	template <typename FuncT>
	Iterator search(FuncT func);
	
	void erase(Iterator iter);
	Iterator insert(Iterator iter, const ElementT& element);
	
private:
	typedef AVLNode<ElementT> Node;
	
	/// Balance a node after erase or insertion to its subtree.
	/// @returns true if the tree had to be balanced, false if it was already
	/// balanced.
	bool balanceNode_(Node* node);
	
	/// The root node of the tree. The pointer is nested through two unique_ptrs
	/// to support moving because iterators must stay valid after move too.
	std::unique_ptr<std::unique_ptr<Node>> root_;
};

}
}
}

#include "avl_tree_impl.hpp"

#endif
