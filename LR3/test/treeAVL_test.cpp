#include "../treeAVL.h"

#include <gtest/gtest.h>

TEST(AvlTreeTest, InsertBasic) {
    AvlTree tree;
    tree.TINSERT(10);
    ASSERT_TRUE(tree.ISMEMBER(10));
    ASSERT_EQ(tree.PRINT_INORDER(), "10");
}

TEST(AvlTreeTest, InsertMultiple) {
    AvlTree tree;
    tree.TINSERT(30);
    tree.TINSERT(10);
    tree.TINSERT(20);
    ASSERT_EQ(tree.PRINT_INORDER(), "10 20 30");
    ASSERT_EQ(tree.PRINT_PREORDER(), "20 10 30");
}

TEST(AvlTreeTest, DeleteLeaf) {
    AvlTree tree;
    tree.TINSERT(1);
    tree.TINSERT(2);
    tree.TINSERT(3);
    tree.TDEL(3);
    ASSERT_FALSE(tree.ISMEMBER(3));
    ASSERT_EQ(tree.PRINT_INORDER(), "1 2");
}

TEST(AvlTreeTest, DeleteRoot) {
    AvlTree tree;
    tree.TINSERT(2);
    tree.TINSERT(1);
    tree.TINSERT(3);
    tree.TDEL(2);
    ASSERT_EQ(tree.PRINT_INORDER(), "1 3");
}

TEST(AvlTreeTest, AllTraversals) {
    AvlTree tree;
    tree.TINSERT(3);
    tree.TINSERT(1);
    tree.TINSERT(2);
    ASSERT_EQ(tree.PRINT_PREORDER(), "2 1 3");
    ASSERT_EQ(tree.PRINT_INORDER(), "1 2 3");
    ASSERT_EQ(tree.PRINT_POSTORDER(), "1 3 2");
    ASSERT_EQ(tree.PRINT_BFS(), "2 1 3");
}

TEST(AvlTreeTest, EmptyTree) {
    AvlTree tree;
    ASSERT_FALSE(tree.ISMEMBER(999));
    ASSERT_EQ(tree.PRINT_INORDER(), "");
}

TEST(AvlTreeTest, BalanceRotations) {
    AvlTree tree;
    tree.TINSERT(3);
    tree.TINSERT(2);
    tree.TINSERT(1);
    ASSERT_EQ(tree.PRINT_INORDER(), "1 2 3");
}
