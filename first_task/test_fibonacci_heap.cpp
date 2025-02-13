#include <gtest/gtest.h>
#include "fibonacci_heap.h"

// Test inserting values into the Fibonacci Heap
TEST(FibonacciHeapTest, Insert) {
  FibonacciHeap heap;
  heap.insert(10);
  heap.insert(5);
  heap.insert(20);

  EXPECT_EQ(heap.getMin()->getKey(), 5); // Min should be 5
}

// Test extracting the minimum element
TEST(FibonacciHeapTest, ExtractMin) {
  FibonacciHeap heap;
  heap.insert(10);
  heap.insert(5);
  heap.insert(20);

  const auto minNode = heap.extractMin();
  EXPECT_EQ(minNode, 5); // Should extract 5
  EXPECT_EQ(heap.getMin()->getKey(), 10); // Next min should be 10
}

// Test inserting multiple elements and extracting them
TEST(FibonacciHeapTest, MultipleExtractMin) {
  FibonacciHeap heap;
  heap.insert(10);
  heap.insert(5);
  heap.insert(15);

  const auto minNode = heap.extractMin();
  ASSERT_NE(minNode, std::nullopt); // Ensure the extracted node is not null
  EXPECT_EQ(minNode, 5);

  Node *nextMin = heap.getMin();
  ASSERT_NE(nextMin, nullptr); // Prevent null pointer dereference
  EXPECT_EQ(nextMin->getKey(), 10);
}

// Test empty heap behavior
TEST(FibonacciHeapTest, EmptyHeap) {
  FibonacciHeap heap;
  EXPECT_TRUE(heap.isEmpty());

  const auto min = heap.extractMin();
  EXPECT_EQ(min, std::nullopt); // Extracting from an empty heap should return nullptr
}

TEST(FibonacciHeapTest, LargeInsertionAndExtraction) {
  FibonacciHeap heap;
  const int numElements = 100;

  // Insert 1000 elements in reverse order
  for (int i = numElements; i > 0; --i) {
    heap.insert(i);
  }

  // Ensure the minimum element is 1
  EXPECT_EQ(heap.getMin()->getKey(), 1);

  // Extract elements one by one and check order
  for (int i = 1; i <= numElements; ++i) {
    auto minNode = heap.extractMin();
    ASSERT_TRUE(minNode.has_value());
    EXPECT_EQ(minNode.value(), i);
  }

  EXPECT_TRUE(heap.isEmpty());
}
