#ifndef FRIVOL_STACK_HPP
#define FRIVOL_STACK_HPP

#include "array.hpp"

namespace frivol {

/// Stack of elements.
/// @tparam T The type of stored elements. Should be default constructible.
template <typename T>
class Stack {
public:
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<T>));
	
	/// Constructs empty stack.
	Stack();
	
	/// Returns true if the stack is empty.
	bool empty() const;
	
	/// Returns reference to the top element of the stack. Call only if empty()
	/// is false.
	T& top();
	
	/// Removes the top element of the stack. Call only if empty() is false.
	void pop();
	
	/// Pushes element to the top of the stack.
	/// @param element The element to push.
	void push(const T& element);
	
private:
	/// Array containing the elements. May be larger than size_.
	Array<T> elements_;
	
	/// Number of currently stored elements.
	Idx size_;
};

}

#include "stack_impl.hpp"

#endif
