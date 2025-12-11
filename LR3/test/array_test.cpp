#include "../array.h"

#include <gtest/gtest.h>

TEST(ArrayTest, Exceptions) {
    Array arr;
    EXPECT_THROW(arr.get(-1), std::out_of_range);
    EXPECT_THROW(arr.get(0), std::out_of_range);
    EXPECT_THROW(arr.removeAt(0), std::out_of_range);
    EXPECT_THROW(arr.insertAt(1, "x"), std::out_of_range);
}

TEST(ArrayTest, SaveLoadTextBinary) {
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");
    arr.saveToText("arr.txt");
    arr.saveToBinary("arr.bin");

    Array arr2;
    arr2.loadFromText("arr.txt");
    EXPECT_EQ(arr2.getSize(), 2);
    EXPECT_EQ(arr2.get(0), "a");

    Array arr3;
    arr3.loadFromBinary("arr.bin");
    EXPECT_EQ(arr3.getSize(), 2);
    EXPECT_EQ(arr3.get(1), "b");
}

TEST(ArrayTest, BasicOperations) {
    Array arr;
    arr.pushBack("1");
    arr.pushBack("3");
    arr.pushFront("0");
    arr.insertAt(2, "2");

    EXPECT_EQ(arr.getSize(), 4);
    EXPECT_EQ(arr.get(0), "0");
    EXPECT_EQ(arr.get(1), "1");
    EXPECT_EQ(arr.get(2), "2");
    EXPECT_EQ(arr.get(3), "3");

    arr.popFront();
    arr.popBack();
    EXPECT_EQ(arr.getSize(), 2);
}
