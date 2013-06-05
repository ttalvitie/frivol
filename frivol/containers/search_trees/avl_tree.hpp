#ifndef FRIVOL_CONTAINERS_SEARCH_TREES_AVL_TREE_HPP
#define FRIVOL_CONTAINERS_SEARCH_TREES_AVL_TREE_HPP

#include <frivol/common.hpp>

namespace frivol {
namespace containers {
namespace search_trees {

// Forward declarations.
namespace avl_ {
	template <typename ElementT>
	struct Node;
}
template <typename ElementT>
class AVLTree;

/// Bidirectional iterator for iterating over the elements of an AVLTree.
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
	typedef avl_::Node<ElementT> Node;
	
	/// Pointer to the AVL tree to which the iterator points.
	AVLTree<ElementT>* tree_;
	
	/// Pointer to the current node, or nullptr if we are past the end.
	Node* node_;
	
	/// Constructs AVL tree iterator.
	/// @param tree Pointer to the AVL tree to which the iterator points.
	/// @param node Pointer to the current node, or nullptr for past the end.
	AVLIterator(AVLTree<ElementT>* tree, Node* node = nullptr);
	
	friend class AVLTree<ElementT>;
};

/// Implementation of SearchTreeConcept using AVL tree. TODO: implement balacing
template <typename ElementT>
class AVLTree {
public:
	typedef AVLIterator<ElementT> Iterator;
	
	bool empty() const;
	
	Iterator begin();
	Iterator end();
	
	template <typename FuncT>
	Iterator search(FuncT func);
	
	void erase(Iterator iter);
	Iterator insert(Iterator iter, const ElementT& element);
	
private:
	typedef avl_::Node<ElementT> Node;
	
	/// Get reference to the unique_ptr containing given node.
	/// @param node Pointer to the node.
	std::unique_ptr<Node>& getOwner_(Node* node);
	
	/// Swap the positions of two nodes, while keeping the elements at the same
	/// node instances.
	/// @param node1,node2 Pointers to the nodes.
	void swapNodes_(Node* node1, Node* node2);
	
	/// Set left child of a node.
	/// @param node Pointer to the node.
	/// @param element The element to contain in the node.
	/// @returns pointer to the added node.
	Node* addLeftChild_(Node* node, const ElementT& element);
	
	/// Set right child of a node.
	/// @param node Pointer to the node.
	/// @param element The element to contain in the node.
	/// @returns pointer to the added node.
	Node* addRightChild_(Node* node, const ElementT& element);
	
	/// The root node of the tree or nullptr if the tree is empty.
	std::unique_ptr<Node> root_;
	
	friend class AVLIterator<ElementT>;
};

}
}
}

#include "avl_tree_impl.hpp"

#endif
