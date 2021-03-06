#ifndef FRIVOL_CONTAINERS_SEARCH_TREE_CONCEPT_HPP
#define FRIVOL_CONTAINERS_SEARCH_TREE_CONCEPT_HPP

#include <boost/concept_check.hpp>

namespace frivol {
namespace containers {

/// Concept checking class for search trees X for elements of type ElementT.
/// Search trees are sequence containers, the elements of which are iterated
/// using iterator objects of type X::Iterator. The iterator must be a
/// standard bidirectional iterator. X must support the following operations:
///  - <construct>() creates empty search tree.
///  - bool empty() const retuns true if the search tree is empty.
///  - Iterator begin() returns the iterator of the first element (or past-the-end
///    if empty).
///  - Iterator end() returns the iterator past the last element.
///  - template<typename FuncT> Iterator search(FuncT func) searches the
///    sequence using the supplied int(Iterator)-function that for given
///    iterator iter returns negative if the searched element is before iter,
///    positive if it is after iter, and 0 if iter is the right element.
///    If an element such that func returns 0 is found, it is returned,
///    otherwise end() is returned.
///  - void erase(Iterator iter) removes element at iter. Other iterators
///    should not be invalidated.
///  - Iterator insert(Iterator iter, const ElementT& elem) inserts elem before
///    iter and returns the iterator of the new element. Does not invalidate any
///    iterators.
/// 
/// X may assume that ElementT is copy constructible.
template <typename X, typename ElementT>
class SearchTreeConcept {
public:
	typedef typename X::Iterator IteratorT;
	BOOST_CONCEPT_ASSERT((boost::CopyConstructible<ElementT>));
	BOOST_CONCEPT_ASSERT((boost::BidirectionalIterator<IteratorT>));
	
	BOOST_CONCEPT_USAGE(SearchTreeConcept) {
		X x;
		IteratorT iter;
		sameType(x.empty(), bool());
		sameType(x.begin(), iter);
		sameType(x.end(), iter);
		x.erase(iter);
		sameType(x.insert(iter, elem), iter);
		x.search([](IteratorT iter) -> int { return 0; });
	}
	
private:
	ElementT elem;
	
	/// Function that causes compile time error if called with parameters of
	/// different types.
	template <typename T>
	void sameType(const T&, const T&);
};

}
}

#endif
