namespace frivol {

template <typename T>
Array<T>::Array(Idx size)
	: elements_(new T[size]),
	  size_(size)
{ }

template <typename T>
const T& Array<T>::operator[](Idx index) const {
#ifdef FRIVOL_ARRAY_BOUNDS_CHECKING
	if(index >= getSize()) {
		throw std::out_of_bounds("Array<T>::operator[] const: Array index out of bounds.");
	}
#endif
	
	return elements_[index];
}

template <typename T>
T& Array<T>::operator[](Idx index) {
#ifdef FRIVOL_ARRAY_BOUNDS_CHECKING
	if(index >= getSize()) {
		throw std::out_of_bounds("Array<T>::operator[]: Array index out of bounds.");
	}
#endif
	
	return elements_[index];
}

template <typename T>
Idx Array<T>::getSize() const {
	return size_;
}

}
