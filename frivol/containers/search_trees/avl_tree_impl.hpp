namespace frivol {
namespace containers {
namespace search_trees {

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
	return node_->getElement();
}

template <typename ElementT>
ElementT* AVLIterator<ElementT>::operator->(){
	return &node_->getElement();
}

template <typename ElementT>
AVLIterator<ElementT>& AVLIterator<ElementT>::operator++() {
	node_ = node_->getNextNode();
	return *this;
}

template <typename ElementT>
AVLIterator<ElementT>& AVLIterator<ElementT>::operator--() {
	if(node_ == nullptr) {
		node_ = (*root_)->getRightmostDescendant();
	} else {
		node_ = node_->getPreviousNode();
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
AVLIterator<ElementT>::AVLIterator(const std::unique_ptr<Node>& root, Node* node)
	: root_(&root),
	  node_(node)
{ }

template <typename ElementT>
AVLTree<ElementT>::AVLTree()
	: root_(new std::unique_ptr<Node>)
{ }

template <typename ElementT>
bool AVLTree<ElementT>::empty() const {
	return root_->get() == nullptr;
}

template <typename ElementT>
AVLIterator<ElementT> AVLTree<ElementT>::begin() {
	if(empty()) return end();
	
	return Iterator(*root_, root_->get()->getLeftmostDescendant());
}

template <typename ElementT>
AVLIterator<ElementT> AVLTree<ElementT>::end() {
	return Iterator(*root_);
}

template <typename ElementT>
template <typename FuncT>
AVLIterator<ElementT> AVLTree<ElementT>::search(FuncT func) {
	Node* node = root_->get();
	while(node != nullptr) {
		int direction = func(Iterator(*root_, node));
		
		if(direction == 0) {
			return Iterator(*root_, node);
		}
		
		if(direction < 0) {
			node = node->getLeftChild();
		} else {
			node = node->getRightChild();
		}
	}
	
	return end();
}

template <typename ElementT>
void AVLTree<ElementT>::erase(Iterator iter) {
	Node* node = iter.node_;
	while(true) {
		// If the node is a leaf, remove it. Otherwise swap with successor or
		// predecessor and retry.
		Node* left = node->getLeftChild();
		Node* right = node->getRightChild();
		if(left == nullptr && right == nullptr) {
			if(node->getParent() == nullptr) {
				root_->reset();
			} else if(node->getParent()->getLeftChild() == node) {
				node->getParent()->removeLeftSubtree();
			} else {
				node->getParent()->removeRightSubtree();
			}
			return;
		}
		
		if(left != nullptr) {
			Node::swapNodes(node, node->getPreviousNode(), *root_);
		} else {
			Node::swapNodes(node, node->getNextNode(), *root_);
		}
		
	}
}

template <typename ElementT>
AVLIterator<ElementT> AVLTree<ElementT>::insert(Iterator iter, const ElementT& element) {
	Node* new_node;
	if(empty()) {
		root_->reset(new Node(element));
		new_node = root_->get();
	} else if(iter.node_ == nullptr) {
		// Add to end.
		new_node = root_->get()->getRightmostDescendant()->createRightChild(element);
	} else {
		// Add before node.
		Node* node = iter.node_;
		
		if(node->getLeftChild() == nullptr) {
			new_node = node->createLeftChild(element);
		} else {
			Node* base = node->getLeftChild()->getRightmostDescendant();
			new_node = base->createRightChild(element);
		}
	}
	
	return Iterator(*root_, new_node);
}

}
}
}
