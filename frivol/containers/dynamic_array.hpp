#ifndef FRIVOL_CONTAINERS_DYNAMIC_ARRAY_HPP
#define FRIVOL_CONTAINERS_DYNAMIC_ARRAY_HPP

#include <frivol/common.hpp>
#include <frivol/containers/array.hpp>

namespace frivol {
namespace containers {

/// Array that is more efficient at adding elements to the end than a regular
/// array because DynamicArray constructs more elements in advance.
/// @tparam T The type of stored elements. Should be default constructible.
template <typename T>
class DynamicArray {
public:
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<T>));
	
	/// Creates a dynamic array of size 0.
	DynamicArray();
	
	/// Creates a dynamic array with all elements default-constructed.
	/// @param size The size of the array.
	DynamicArray(Idx size);
	
	/// @{
	/// Returns reference to an element in the array.
	/// @param index The zero-based index of the element.
	/// @throws std::out_of_range if FRIVOL_ARRAY_BOUNDS_CHECKING is defined and 'index' overflows.
	const T& operator[](Idx index) const;
	T& operator[](Idx index);
	/// @}
	
	/// Returns the size of the array.
	Idx getSize() const;
	
	/// Adds given element to the end of the dynamic array, increasing its size
	/// by one.
	/// @param element The element to add.
	/// @returns the index of the added element.
	Idx add(const T& element);
	
private:
	/// Array containing the elements. May be larger than size_.
	Array<T> elements_;
	
	/// The size of the dynamic array.
	Idx size_;
};

}
}

#include "dynamic_array_impl.hpp"

#endif
