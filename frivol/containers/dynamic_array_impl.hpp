namespace frivol {
namespace containers {

template <typename T>
DynamicArray<T>::DynamicArray()
	: elements_(0),
	  size_(0)
{ }

template <typename T>
DynamicArray<T>::DynamicArray(Idx size)
	: elements_(size),
	  size_(size)
{
	
}

template <typename T>
const T& DynamicArray<T>::operator[](Idx index) const {
#ifdef FRIVOL_ARRAY_BOUNDS_CHECKING
	if(index >= getSize()) {
		throw std::out_of_range("DynamicArray<T>::operator[] const: Array index out of bounds.");
	}
#endif
	
	return elements_[index];
}

template <typename T>
T& DynamicArray<T>::operator[](Idx index) {
#ifdef FRIVOL_ARRAY_BOUNDS_CHECKING
	if(index >= getSize()) {
		throw std::out_of_range("DynamicArray<T>::operator[] const: Array index out of bounds.");
	}
#endif
	
	return elements_[index];
}

template <typename T>
Idx DynamicArray<T>::getSize() const {
	return size_;
}

template <typename T>
Idx DynamicArray<T>::add(const T& element) {
	if(elements_.getSize() == size_) {
		if(size_ == 0) {
			elements_.resize(1);
		} else {
			elements_.resize(2 * size_);
		}
	}
	
	elements_[size_] = element;
	++size_;
}

}
}
