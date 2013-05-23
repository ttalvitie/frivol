namespace frivol {
namespace containers {

template <typename T>
Array<T>::Array(Idx size)
	: elements_(new T[size]),
	  size_(size)
{ }

template <typename T>
const T& Array<T>::operator[](Idx index) const {
#ifdef FRIVOL_ARRAY_BOUNDS_CHECKING
	if(index >= getSize()) {
		throw std::out_of_range("Array<T>::operator[] const: Array index out of bounds.");
	}
#endif
	
	return elements_[index];
}

template <typename T>
T& Array<T>::operator[](Idx index) {
#ifdef FRIVOL_ARRAY_BOUNDS_CHECKING
	if(index >= getSize()) {
		throw std::out_of_range("Array<T>::operator[]: Array index out of bounds.");
	}
#endif
	
	return elements_[index];
}

template <typename T>
Idx Array<T>::getSize() const {
	return size_;
}

template <typename T>
void Array<T>::resize(Idx size) {
	BOOST_CONCEPT_ASSERT((boost::Assignable<T>));
	
	std::unique_ptr<T[]> new_elements(new T[size]);
	for(Idx i = 0; i < size_ && i < size; ++i) {
		new_elements[i] = elements_[i];
	}
	
	size_ = size;
	elements_ = std::move(new_elements);
}

}
}
