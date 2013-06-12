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

BOOST_AUTO_TEST_CASE_TEMPLATE(iteration_works2, SearchTree, SearchTreeTypes) {
	SearchTree t;
	auto iter = t.end();
	iter = t.insert(iter, 0);
	auto iter2 = t.insert(iter, 1);
	t.insert(iter, 2);
	t.insert(iter2, 3);
	t.insert(iter2, 4);
	t.erase(iter2);
	
	iter = t.begin();
	BOOST_CHECK_EQUAL(*iter++, 3);
	BOOST_CHECK_EQUAL(*iter++, 4);
	BOOST_CHECK_EQUAL(*iter++, 2);
	BOOST_CHECK_EQUAL(*iter++, 0);
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

void checkAVLTreeBalanced(AVLTree<int>& tree) {
	int size = 0;
	int max_queries = 0;
	for(auto iter = tree.begin(); iter != tree.end(); ++iter) {
		int queries = 0;
		auto find_iter = tree.search([&](AVLTree<int>::Iterator search_iter) {
			++queries;
			return *iter - *search_iter;
		});
		BOOST_CHECK(find_iter != tree.end());
		max_queries = std::max(max_queries, queries);
		
		++size;
	}
	
	// There should be only a 2 * log_2(size + 1) queries.
	BOOST_CHECK(1 << max_queries <= (size + 1) * (size + 1));
}

BOOST_AUTO_TEST_CASE(avl_tree_balanced_when_adding_in_increasing_order) {
	AVLTree<int> tree;
	checkAVLTreeBalanced(tree);
	for(int i = 0; i < 1000; ++i) {
		tree.insert(tree.end(), i);
		checkAVLTreeBalanced(tree);
	}
}

BOOST_AUTO_TEST_CASE(avl_tree_balanced_when_adding_in_decreasing_order) {
	AVLTree<int> tree;
	checkAVLTreeBalanced(tree);
	for(int i = 999; i >= 0; --i) {
		tree.insert(tree.begin(), i);
		checkAVLTreeBalanced(tree);
	}
}

BOOST_AUTO_TEST_SUITE_END()
