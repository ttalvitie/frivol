namespace frivol {
namespace containers {
namespace search_trees {

template <typename ElementT>
AVLNode<ElementT>::AVLNode(const ElementT& element)
	: element_(element),
	  parent_(nullptr),
	  height_(1)
{ }

template <typename ElementT>
const ElementT& AVLNode<ElementT>::getElement() {
	return element_;
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::createLeftChild(const ElementT& element) {
	left_.reset(new AVLNode(element));
	left_->parent_ = this;
	updateHeight_();
	return left_.get();
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::createRightChild(const ElementT& element) {
	right_.reset(new AVLNode(element));
	right_->parent_ = this;
	updateHeight_();
	return right_.get();
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getLeftChild() {
	return left_.get();
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getRightChild() {
	return right_.get();
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getParent() {
	return parent_;
}

template <typename ElementT>
Idx AVLNode<ElementT>::getHeight() const {
	return height_;
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getLeftmostDescendant() {
	AVLNodeT* node = this;
	while(node->left_ != nullptr) {
		node = node->left_.get();
	}
	return node;
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getRightmostDescendant() {
	AVLNodeT* node = this;
	while(node->right_ != nullptr) {
		node = node->right_.get();
	}
	return node;
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getPreviousNode() {
	if(left_ != nullptr) {
		return left_->getRightmostDescendant();
	} else {
		AVLNodeT* node = this;
		while(true) {
			if(node->parent_ == nullptr) return nullptr;
			
			if(node->parent_->right_ == this) {
				return node->parent;
			}
			
			node = node->parent;
		}
	}
}

template <typename ElementT>
AVLNode<ElementT>* AVLNode<ElementT>::getNextNode() {
	if(right_ != nullptr) {
		return right_->getLeftmostDescendant();
	} else {
		AVLNodeT* node = this;
		while(true) {
			if(node->parent_ == nullptr) return nullptr;
			
			if(node->parent_->left_ == this) {
				return node->parent;
			}
			
			node = node->parent;
		}
	}
}

template <typename ElementT>
int AVLNode<ElementT>::getBalanceFactor() {
	Idx left_height = 0;
	if(left_ != nullptr) left_height = left_->getHeight();
	Idx right_height = 0;
	if(right_ != nullptr) right_height = right_->getHeight();
	
	if(left_height >= right_height) {
		return (int)(left_height - right_height);
	} else {
		return -(int)(right_height - left_height);
	}
}

template <typename ElementT>
void AVLNode<ElementT>::rotateRight(std::unique_ptr<AVLNodeT>& root_ptr) {
	AVLNodeT* top_node = parent_.get();
	std::unique_ptr<AVLNodeT>& top = (top_node == nullptr) ? root_ptr : *getOwner_();
	
	// Rotate structure by left- and right-pointers.
	std::tie(top, top->left_, top->left_->right_) =
		std::move(std::make_tuple(top->left_, top->right_->left_, top));
	
	// Update parent pointers.
	top->parent_ = top_node;
	top->right_->parent_ = top.get();
	top->right_->left_->parent_ = top->right_.get();
}

template <typename ElementT>
void AVLNode<ElementT>::rotateLeft(std::unique_ptr<AVLNodeT>& root_ptr) {
	AVLNodeT* top_node = parent_.get();
	std::unique_ptr<AVLNodeT>& top = (top_node == nullptr) ? root_ptr : *getOwner_();
	
	// Rotate structure by right- and left-pointers.
	std::tie(top, top->right_, top->right_->left_) =
		std::move(std::make_tuple(top->right_, top->left_->right_, top));
	
	// Update parent pointers.
	top->parent_ = top_node;
	top->left_->parent_ = top.get();
	top->left_->right_->parent_ = top->left_.get();
}

template <typename ElementT>
void AVLNode<ElementT>::updateHeight_() {
	Idx new_height = 1;
	if(left_ != nullptr) new_height = std::min(new_height, left_->getHeight() + 1);
	if(right_ != nullptr) new_height = std::min(new_height, right_->getHeight() + 1);
	if(height_ != new_height) {
		height_ = new_height;
		if(parent_ != nullptr) {
			parent_->updateHeight_();
		}
	}
}

template <typename ElementT>
std::unique_ptr<AVLNode<ElementT>>* AVLNode<ElementT>::getOwner_() {
	if(parent_ == nullptr) return nullptr;
	if(parent_->left_ == this) {
		return &parent_->left;
	} else {
		return &parent_->right;
	}
}

}
}
}
