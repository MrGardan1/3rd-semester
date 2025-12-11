#include "../stack.h"

#include <gtest/gtest.h>

TEST(StackTest, EmptyStack) {
    Stack s;
    EXPECT_EQ(s.getSize(), 0);
    EXPECT_EQ(s.pop(), "");
    EXPECT_EQ(s.peek(), "");
}

TEST(StackTest, ResizeInternalBuffer) {
    Stack s(1);
    s.push("a");
    s.push("b");
    s.push("c");
    EXPECT_EQ(s.getSize(), 3);
    EXPECT_EQ(s.pop(), "c");
    EXPECT_EQ(s.pop(), "b");
    EXPECT_EQ(s.pop(), "a");
}

TEST(StackTest, SaveLoadText) {
    Stack s;
    s.push("one");
    s.push("two");
    s.saveToText("stack_txt.txt");

    Stack s2;
    s2.loadFromText("stack_txt.txt");
    EXPECT_EQ(s2.getSize(), 2);
    EXPECT_EQ(s2.pop(), "two");
    EXPECT_EQ(s2.pop(), "one");
}

TEST(StackTest, SaveLoadBinary) {
    Stack s;
    s.push("x");
    s.push("y");
    s.saveToBinary("stack_bin.dat");

    Stack s2;
    s2.loadFromBinary("stack_bin.dat");
    EXPECT_EQ(s2.getSize(), 2);
    EXPECT_EQ(s2.pop(), "y");
    EXPECT_EQ(s2.pop(), "x");
}

TEST(StackTest, LoadFromNonExistingFiles) {
    Stack s;
    s.loadFromText("no_such_file.txt");
    s.loadFromBinary("no_such_file.bin");
    EXPECT_EQ(s.getSize(), 0);
}

TEST(StackTest, ZeroInitialCapacity) {
    Stack s(0);
    s.push("a");
    EXPECT_EQ(s.getSize(), 1);
}
