#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <frivol/containers/search_trees/avl_node.hpp>

using namespace frivol;
using namespace frivol::containers;
using namespace frivol::containers::search_trees;

typedef AVLNode<int> AVLNodeT;

BOOST_AUTO_TEST_SUITE(avl_node)

void assertSubtreesEqual(AVLNodeT* tree1, AVLNodeT* tree2, bool not_a_root = false) {
	BOOST_CHECK_EQUAL(tree1 == nullptr, tree2 == nullptr);
	if(tree1 == nullptr) return;
	
	BOOST_CHECK_EQUAL(tree1->getElement(), tree2->getElement());
	
	if(not_a_root) {
		BOOST_CHECK(tree1->getParent() != nullptr);
		BOOST_CHECK(tree2->getParent() != nullptr);
	}
	BOOST_CHECK(
		tree1->getParent() == nullptr ||
		tree1->getParent()->getLeftChild() == tree1 ||
		tree1->getParent()->getLeftChild() == tree1
	);
	BOOST_CHECK(
		tree2->getParent() == nullptr ||
		tree2->getParent()->getLeftChild() == tree2 ||
		tree2->getParent()->getLeftChild() == tree2
	);
	
	assertSubtreesEqual(tree1->getLeftChild(), tree2->getLeftChild(), true);
	assertSubtreesEqual(tree1->getRightChild(), tree2->getRightChild(), true);
}

BOOST_AUTO_TEST_CASE(basic_construction_works) {
	AVLNodeT root(0);
	AVLNodeT* node = &root;
	AVLNodeT* nodeR = node->createRightChild(1);
	AVLNodeT* nodeL = node->createLeftChild(2);
	AVLNodeT* nodeLL = nodeL->createLeftChild(3);
	AVLNodeT* nodeRL = nodeR->createLeftChild(4);
	AVLNodeT* nodeRR = nodeR->createRightChild(5);
	
	BOOST_CHECK_EQUAL(node->getElement(), 0);
	BOOST_CHECK_EQUAL(nodeR->getElement(), 1);
	BOOST_CHECK_EQUAL(nodeL->getElement(), 2);
	BOOST_CHECK_EQUAL(nodeLL->getElement(), 3);
	BOOST_CHECK_EQUAL(nodeRL->getElement(), 4);
	BOOST_CHECK_EQUAL(nodeRR->getElement(), 5);
	
	BOOST_CHECK_EQUAL(node->getParent(), (AVLNodeT*)nullptr);
	BOOST_CHECK_EQUAL(nodeR->getParent(), node);
	BOOST_CHECK_EQUAL(nodeL->getParent(), node);
	BOOST_CHECK_EQUAL(nodeLL->getParent(), nodeL);
	BOOST_CHECK_EQUAL(nodeRL->getParent(), nodeR);
	BOOST_CHECK_EQUAL(nodeRR->getParent(), nodeR);
	
	BOOST_CHECK_EQUAL(node->getLeftChild(), nodeL);
	BOOST_CHECK_EQUAL(nodeR->getLeftChild(), nodeRL);
	BOOST_CHECK_EQUAL(nodeL->getLeftChild(), nodeLL);
	BOOST_CHECK_EQUAL(nodeLL->getLeftChild(), (AVLNodeT*)nullptr);
	BOOST_CHECK_EQUAL(nodeRL->getLeftChild(), (AVLNodeT*)nullptr);
	BOOST_CHECK_EQUAL(nodeRR->getLeftChild(), (AVLNodeT*)nullptr);
	
	BOOST_CHECK_EQUAL(node->getRightChild(), nodeR);
	BOOST_CHECK_EQUAL(nodeR->getRightChild(), nodeRR);
	BOOST_CHECK_EQUAL(nodeL->getRightChild(), (AVLNodeT*)nullptr);
	BOOST_CHECK_EQUAL(nodeLL->getRightChild(), (AVLNodeT*)nullptr);
	BOOST_CHECK_EQUAL(nodeRL->getRightChild(), (AVLNodeT*)nullptr);
	BOOST_CHECK_EQUAL(nodeRR->getRightChild(), (AVLNodeT*)nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
