#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <frivol/containers/search_tree_concept.hpp>

#include <frivol/containers/search_trees/dummy_search_tree.hpp>
#include <frivol/containers/search_trees/avl_tree.hpp>

using namespace frivol;
using namespace frivol::containers;
using namespace frivol::containers::search_trees;

BOOST_AUTO_TEST_SUITE(search_tree)

typedef boost::mpl::list<DummySearchTree<int>, AVLTree<int>> SearchTreeTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(implements_concept, SearchTree, SearchTreeTypes) {
	BOOST_CONCEPT_ASSERT((SearchTreeConcept<SearchTree, int>));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_insert_erase_empty, SearchTree, SearchTreeTypes) {
	SearchTree t;
	BOOST_CHECK(t.empty());
	t.insert(t.end(), 51);
	BOOST_CHECK(!t.empty());
	t.insert(t.begin(), 1);
	BOOST_CHECK(!t.empty());
	t.erase(t.begin());
	BOOST_CHECK(!t.empty());
	t.erase(t.begin());
	BOOST_CHECK(t.empty());
	t.insert(t.begin(), 5);
	BOOST_CHECK(!t.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(iteration_works, SearchTree, SearchTreeTypes) {
	SearchTree t;
	typename SearchTree::Iterator iter = t.end();
	t.insert(iter, 5);
	t.insert(iter, 8);
	--iter;
	t.insert(iter, 3);
	--iter;
	--iter;
	BOOST_CHECK(iter == t.begin());
	
	BOOST_CHECK_EQUAL(*iter++, 5);
	BOOST_CHECK_EQUAL(*iter++, 3);
	BOOST_CHECK_EQUAL(*iter++, 8);
	
	BOOST_CHECK(iter == t.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(search_positive_works, SearchTree, SearchTreeTypes) {
	SearchTree t;
	typedef typename SearchTree::Iterator Iterator;
	t.insert(t.end(), 1);
	t.insert(t.end(), 4);
	t.insert(t.end(), 6);
	t.insert(t.end(), 7);
	t.insert(t.end(), 9);
	Iterator result = t.search([](Iterator iter) { return 6 - *iter; });
	
	BOOST_CHECK(result != t.end());
	BOOST_CHECK_EQUAL(*result, 6);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(search_negative_works, SearchTree, SearchTreeTypes) {
	SearchTree t;
	typedef typename SearchTree::Iterator Iterator;
	t.insert(t.end(), 1);
	t.insert(t.end(), 4);
	t.insert(t.end(), 6);
	t.insert(t.end(), 7);
	t.insert(t.end(), 9);
	Iterator result = t.search([](Iterator iter) { return 8 - *iter; });
	
	BOOST_CHECK(result == t.end());
}

BOOST_AUTO_TEST_SUITE_END()
