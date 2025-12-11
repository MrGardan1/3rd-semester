#include "../hashTable.h"

#include <gtest/gtest.h>

TEST(HashTableTest, PutGet) {
    HashTable table;
    table.put("key1", "value1");
    table.put("key2", "value2");
    ASSERT_EQ(table.get("key1"), "value1");
    ASSERT_EQ(table.get("key2"), "value2");
}

TEST(HashTableTest, Remove) {
    HashTable table;
    table.put("test", "data");
    table.remove("test");
    ASSERT_FALSE(table.get("test") != "");
    ASSERT_EQ(table.getSize(), 0);
}

TEST(HashTableTest, EmptyHash) {
    HashTable table;
    ASSERT_EQ(table.get("missing"), "");
    ASSERT_EQ(table.getSize(), 0);
}

TEST(HashTableTest, UpdateValue) {
    HashTable table;
    table.put("key", "old");
    table.put("key", "new");
    ASSERT_EQ(table.get("key"), "new");
    ASSERT_EQ(table.getSize(), 1);
}

TEST(HashTableTest, MultipleCollisions) {
    HashTable table(2);
    table.put("aa", "1");
    table.put("bb", "2");
    ASSERT_EQ(table.getSize(), 2);
}
