#include "../queue.h"

#include <gtest/gtest.h>

TEST(QueueTest, PushPop) {
    Queue q;
    q.push("1");
    q.push("2");
    q.push("3");
    ASSERT_EQ(q.pop(), "1");
    ASSERT_EQ(q.pop(), "2");
    ASSERT_EQ(q.getSize(), 1);
}

TEST(QueueTest, EmptyQueue) {
    Queue q;
    ASSERT_EQ(q.pop(), "");
    ASSERT_EQ(q.peek(), "");
    ASSERT_EQ(q.getSize(), 0);
}

TEST(QueueTest, MultipleOperations) {
    Queue q;
    q.push("a");
    q.push("b");
    q.pop();
    q.push("c");
    ASSERT_EQ(q.pop(), "b");
    ASSERT_EQ(q.peek(), "c");
}

TEST(QueueTest, Resize) {
    Queue q(1);
    q.push("1");
    q.push("2");
    ASSERT_EQ(q.getSize(), 2);
}

TEST(QueueTest, Peek) {
    Queue q;
    q.push("test");
    ASSERT_EQ(q.peek(), "test");
    ASSERT_EQ(q.getSize(), 1);
}
