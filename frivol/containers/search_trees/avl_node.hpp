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
	typedef AVLNode<ElementT> Node;
	
	/// Constructs an AVL root node with no children.
	AVLNode(const ElementT& element);
	
	/// Returns reference to the element stored in the node.
	ElementT& getElement();
	
	/// Create a new node and place it as the left child of this node.
	/// @param element The element stored in the new node.
	/// @returns pointer to the added node.
	Node* createLeftChild(const ElementT& element);
	
	/// Create a new node and place it as the right child of this node.
	/// @param element The element stored in the new node.
	/// @returns pointer to the added node.
	Node* createRightChild(const ElementT& element);
	
	/// Remove the node from the tree if it has at most one child. It will be
	/// replaced by the child.
	/// @param root_ptr This unique pointer should own the root node of the
	/// tree, and will be set to the new root if the root changes.
	/// @returns true if the node was removed, false if the node had both children.
	bool remove(std::unique_ptr<Node>& root_ptr);
	
	/// Returns pointer to the left child or nullptr if none.
	Node* getLeftChild();
	
	/// Returns pointer to the right child or nullptr if none.
	Node* getRightChild();
	
	/// Returns pointer to the parent node or nullptr if this is the root.
	Node* getParent();
	
	/// Returns the height of the subtree from this node, including this node
	/// (leaf nodes have subtree height 1).
	Idx getHeight() const;
	
	/// Returns the in-order leftmost node in the subtree from this node.
	Node* getLeftmostDescendant();
	
	/// Returns the in-order rightmost node in the subtree from this node.
	Node* getRightmostDescendant();
	
	/// Returns the in-order previous node in the tree, or nullptr if the node
	/// is the leftmost.
	Node* getPreviousNode();
	
	/// Returns the in-order next node in the tree, or nullptr if the node
	/// is the rightmost.
	Node* getNextNode();
	
	/// Returns the balance factor of the node, i.e. difference of the heights
	/// of the subtrees from the left node and the right node. Empty subtree has
	/// height 0.
	int getBalanceFactor();
	
	/// Perform right-rotation rooted in this node. Assumes that the node has a
	/// left child.
	/// @param root_ptr This unique pointer should own the root node of the
	/// tree, and will be set to the new root if the root changes.
	void rotateRight(std::unique_ptr<Node>& root_ptr);
	
	/// Perform left-rotation rooted in this node. Assumes that the node has a
	/// right child.
	/// @param root_ptr This unique pointer should own the root node of the
	/// tree, and will be set to the new root if the root changes.
	void rotateLeft(std::unique_ptr<Node>& root_ptr);
	
	/// Swap the positions of two nodes in the tree.
	/// @param node1,node2 Pointers to the nodes to swap.
	/// @param root_ptr This unique pointer should own the root node of the
	/// tree, and will be set to the new root if the root changes.
	static void swapNodes(Node* node1, Node* node2, std::unique_ptr<Node>& root_ptr);
	
private:
	/// Recalculates the height from the heights of the children, and if it
	/// changes, recurse to ancestors.
	void updateHeight_();
	
	/// Returns the unique_ptr owning this node in the tree, or root_ptr if this
	/// is the root.
	/// @param root_ptr The reference returned if the node is root.
	std::unique_ptr<AVLNode>& getOwner_(std::unique_ptr<Node>& root_ptr);
	
	/// The element stored in the node.
	ElementT element_;
	
	/// Parent node of the node or nullptr if the node is a root node.
	Node* parent_;
	
	/// Left child or nullptr if none.
	std::unique_ptr<Node> left_;
	
	/// Right child or nullptr if none.
	std::unique_ptr<Node> right_;
	
	/// Height of the subtree, including this node.
	Idx height_;
};

}
}
}

#include "avl_node_impl.hpp"

#endif
