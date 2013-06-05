namespace frivol {
namespace containers {
namespace search_trees {

namespace avl_ {
template <typename ElementT>
struct Node {
	/// Construct root node.
	/// @param element The element to store in the node.
	Node(const ElementT& element)
		: element(element),
		  parent(nullptr)
	{ }
	
	/// Element stored in the node.
	ElementT element;
	
	/// Left child of the node or nullptr if none.
	std::unique_ptr<Node> left;
	
	/// Right child of the node or nullptr if none.
	std::unique_ptr<Node> right;
	
	/// Parent of the node or nullptr in root node.
	Node* parent;
	
	/// Height of the subtree, including this node.
//	Idx height;
};
}

template <typename ElementT>
bool AVLIterator<ElementT>::operator==(const AVLIterator<ElementT>& other) const {
	return node_ == other.node_;
}

template <typename ElementT>
bool AVLIterator<ElementT>::operator!=(const AVLIterator<ElementT>& other) const {
	return node_ != other.node_;
}

template <typename ElementT>
ElementT& AVLIterator<ElementT>::operator*() {
	return node_->element;
}

template <typename ElementT>
ElementT* AVLIterator<ElementT>::operator->() {
	return &node_->element;
}

template <typename ElementT>
AVLIterator<ElementT>& AVLIterator<ElementT>::operator++() {
	if(node_->right.get() == nullptr) {
		// Nothing on the right, go to first ancestor that has current subtree
		// on the left.
		while(true) {
			if(node_->parent == nullptr) {
				node_ = nullptr;
				break;
			}
			
			if(node_ == node_->parent->left.get()) {
				node_ = node_->parent;
				break;
			}
			
			node_ = node_->parent;
		}
	} else {
		// Next element is the leftmost element in the right subtree.
		node_ = node_->right.get();
		while(node_->left.get() != nullptr) {
			node_ = node_->left.get();
		}
	}
	
	return *this;
}

template <typename ElementT>
AVLIterator<ElementT>& AVLIterator<ElementT>::operator--() {
	// If we are past the end, the previous is the rightmost node.
	if(node_ == nullptr) {
		node_ = tree_->root_.get();
		while(node_->right.get() != nullptr) {
			node_ = node_->right.get();
		}
		return *this;
	}
	
	if(node_->left.get() == nullptr) {
		// Nothing on the left, go to first ancestor that has current subtree
		// on the right.
		while(true) {
			if(node_->parent == nullptr) {
				node_ = nullptr;
				break;
			}
			
			if(node_ == node_->parent->right.get()) {
				node_ = node_->parent;
				break;
			}
			
			node_ = node_->parent;
		}
	} else {
		// Previous element is the rightmost element in the left subtree.
		node_ = node_->left.get();
		while(node_->right.get() != nullptr) {
			node_ = node_->right.get();
		}
	}
	
	return *this;
}

template <typename ElementT>
AVLIterator<ElementT> AVLIterator<ElementT>::operator++(int) {
	AVLIterator<ElementT> ret = *this;
	++(*this);
	return ret;
}

template <typename ElementT>
AVLIterator<ElementT> AVLIterator<ElementT>::operator--(int) {
	AVLIterator<ElementT> ret = *this;
	--(*this);
	return ret;
}

template <typename ElementT>
AVLIterator<ElementT>::AVLIterator(AVLTree<ElementT>* tree, Node* node)
	: tree_(tree),
	  node_(node)
{ }

template <typename ElementT>
bool AVLTree<ElementT>::empty() const {
	return root_.get() == nullptr;
}

template <typename ElementT>
AVLIterator<ElementT> AVLTree<ElementT>::begin() {
	if(empty()) return end();
	
	Node* node = root_.get();
	while(node->left.get() != nullptr) {
		node = node->left.get();
	}
	return Iterator(this, node);
}

template <typename ElementT>
AVLIterator<ElementT> AVLTree<ElementT>::end() {
	return Iterator(this);
}

template <typename ElementT>
template <typename FuncT>
AVLIterator<ElementT> AVLTree<ElementT>::search(FuncT func) {
	Node* node = root_.get();
	while(node != nullptr) {
		Iterator iter(this, node);
		int direction = func(iter);
		if(direction == 0) {
			return iter;
		}
		
		if(direction < 0) {
			node = node->left.get();
		} else {
			node = node->right.get();
		}
	}
	return end();
}

template <typename ElementT>
void AVLTree<ElementT>::erase(Iterator iter) {
	Node* node = iter.node_;
	
	if(node->left.get() == nullptr) {
		if(node->right.get() == nullptr) {
			// Leaf node, just remove.
			getOwner_(node).reset();
		} else {
			// Only right child, replace this with it.
			getOwner_(node) = std::move(node->right);
		}
	} else {
		if(node->right.get() == nullptr) {
			// Only left child, replace this with it.
			getOwner_(node) = std::move(node->left);
		} else {
			// Has both children, swap the node with in-order successor and
			// erase the node there again.
			Iterator swapiter = iter;
			++swapiter;
			swapNodes_(iter.node_, swapiter.node_);
			erase(iter);
		}
	}
}

template <typename ElementT>
AVLIterator<ElementT> AVLTree<ElementT>::insert(Iterator iter, const ElementT& element) {
	if(iter.node_ == nullptr) {
		// Insert to the end.
		if(root_.get() == nullptr) {
			root_.reset(new Node(element));
			return Iterator(this, root_.get());
		} else {
			Node* node = root_.get();
			while(node != nullptr && node->right.get() != nullptr) {
				node = node->right.get();
			}
			return Iterator(this, addRightChild_(node, element));
		}
	} else {
		// Insert before node.
		Node* node = iter.node_;
		if(node->left.get() == nullptr) {
			return Iterator(this, addLeftChild_(node, element));
		} else {
			node = node->left.get();
			while(node->right.get() != nullptr) {
				node = node->right.get();
			}
			return Iterator(this, addRightChild_(node, element));
		}
	}
}

template <typename ElementT>
std::unique_ptr<avl_::Node<ElementT>>& AVLTree<ElementT>::getOwner_(Node* node) {
	if(node->parent == nullptr) {
		return root_;
	} else {
		if(node == node->parent->left.get()) {
			return node->parent->left;
		} else {
			return node->parent->right;
		}
	}
}

template <typename ElementT>
void AVLTree<ElementT>::swapNodes_(Node* node1, Node* node2) {
	std::swap(node1->left, node2->left);
	std::swap(node1->right, node2->right);
	std::swap(node1->parent, node2->parent);
	//std::swap(node1->height, node2->height);
}

template <typename ElementT>
avl_::Node<ElementT>* AVLTree<ElementT>::addLeftChild_(
	Node* node,
	const ElementT& element
) {
	node->left.reset(new Node(element));
	node->left->parent = node;
	return node->left.get();
}

template <typename ElementT>
avl_::Node<ElementT>* AVLTree<ElementT>::addRightChild_(
	Node* node,
	const ElementT& element
) {
	node->right.reset(new Node(element));
	node->right->parent = node;
	return node->right.get();
}

}
}
}
