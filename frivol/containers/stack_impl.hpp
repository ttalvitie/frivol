namespace frivol {
namespace containers {

template <typename T>
Stack<T>::Stack()
	: elements_(1),
	  size_(0)
{ }

template <typename T>
bool Stack<T>::empty() const {
	return size_ == 0;
}

template <typename T>
T& Stack<T>::top() {
	return elements_[size_ - 1];
}

template <typename T>
void Stack<T>::pop() {
	--size_;
}

template <typename T>
void Stack<T>::push(const T& element) {
	++size_;
	if(size_ > elements_.getSize()) {
		elements_.resize(2 * elements_.getSize());
	}
	elements_[size_ - 1] = element;
}

}
}
