#ifndef FRIVOL_CONTAINERS_SEARCH_TREES_AVL_NODE_HPP
#define FRIVOL_CONTAINERS_SEARCH_TREES_AVL_NODE_HPP

#include <frivol/common.hpp>

#include <memory>

namespace frivol {
namespace containers {
namespace search_trees {

/// AVL tree node. Nodes form a rooted binary tree.
/// @tparam ElementT The element type stored in the nodes.
template <typename ElementT>
class AVLNode {
public:
	typedef AVLNode<ElementT> AVLNodeT;
	
	/// Constructs an AVL root node with no children.
	AVLNode(const ElementT& element);
	
	/// Returns the element stored in the node.
	const ElementT& getElement();
	
	/// Create a new node and place it as the left child of this node.
	/// @param element The element stored in the new node.
	/// @returns pointer to the added node.
	AVLNodeT* createLeftChild(const ElementT& element);
	
	/// Create a new node and place it as the right child of this node.
	/// @param element The element stored in the new node.
	/// @returns pointer to the added node.
	AVLNodeT* createRightChild(const ElementT& element);
	
	/// Returns pointer to the left child or nullptr if none.
	AVLNodeT* getLeftChild();
	
	/// Returns pointer to the right child or nullptr if none.
	AVLNodeT* getRightChild();
	
	/// Returns pointer to the parent node or nullptr if this is the root.
	AVLNodeT* getParent();
	
	/// Returns the height of the subtree from this node, including this node
	/// (leaf nodes have subtree height 1).
	Idx getHeight() const;
	
	/// Returns the in-order leftmost node in the subtree from this node.
	AVLNodeT* getLeftmostDescendant();
	
	/// Returns the in-order rightmost node in the subtree from this node.
	AVLNodeT* getRightmostDescendant();
	
	/// Returns the in-order previous node in the tree, or nullptr if the node
	/// is the leftmost.
	AVLNodeT* getPreviousNode();
	
	/// Returns the in-order next node in the tree, or nullptr if the node
	/// is the rightmost.
	AVLNodeT* getNextNode();
	
	/// Returns the balance factor of the node, i.e. difference of the heights
	/// of the subtrees from the left node and the right node. Empty subtree has
	/// height 0.
	int getBalanceFactor();
	
	/// Perform right-rotation rooted in this node. Assumes that the node has a
	/// left child.
	/// @param root_ptr This unique pointer should own the root node of the
	/// tree, and will be set to the new root if the root changes.
	void rotateRight(std::unique_ptr<AVLNodeT>& root_ptr);
	
	/// Perform left-rotation rooted in this node. Assumes that the node has a
	/// right child.
	/// @param root_ptr This unique pointer should own the root node of the
	/// tree, and will be set to the new root if the root changes.
	void rotateLeft(std::unique_ptr<AVLNodeT>& root_ptr);
	
private:
	/// Recalculates the height from the heights of the children, and if it
	/// changes, recurse to ancestors.
	void updateHeight_();
	
	/// Returns the unique_ptr owning this node in the tree, or nullptr if this
	/// is the root.
	std::unique_ptr<AVLNode>* getOwner_();
	
	/// The element stored in the node.
	ElementT element_;
	
	/// Parent node of the node or nullptr if the node is a root node.
	AVLNodeT* parent_;
	
	/// Left child or nullptr if none.
	std::unique_ptr<AVLNodeT> left_;
	
	/// Right child or nullptr if none.
	std::unique_ptr<AVLNodeT> right_;
	
	/// Height of the subtree, including this node.
	Idx height_;
};

}
}
}

#include "avl_node_impl.hpp"

#endif