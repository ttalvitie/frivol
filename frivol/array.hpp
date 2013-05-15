#ifndef FRIVOL_ARRAY_HPP
#define FRIVOL_ARRAY_HPP

#include "common.hpp"

#include <boost/concept_check.hpp>

#include <memory>
#include <stdexcept>

namespace frivol {

/// Simple fixed-size array of elements of type T.
template <typename T>
class Array {
public:
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<T>));
	
	/// Creates an array with all elements default-constructed.
	/// @param size The size of the array.
	Array(Idx size);
	
	/// @{
	/// Returns reference to an element in the array.
	/// @param index The zero-based index of the element.
	/// @throws std::out_of_range if FRIVOL_ARRAY_BOUNDS_CHECKING is defined and 'index' overflows.
	const T& operator[](Idx index) const;
	T& operator[](Idx index);
	/// @}
	
	/// Returns the size of the array.
	Idx getSize() const;
	
	/// Resizes the array to size. If size decreases the extra elements are
	/// removed. If size increases, the new elements are default-constructed.
	/// The operation may assign the current elements to a new place, and
	/// therefore pointers to the array may be invalidated.
	/// @param size The new size.
	void resize(Idx size);
	
private:
	/// The array of elements in the array.
	std::unique_ptr<T[]> elements_;
	
	/// Number of elements in the array.
	Idx size_;
};

}

#include "array_impl.hpp"

#endif
