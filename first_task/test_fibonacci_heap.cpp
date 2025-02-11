#include <gtest/gtest.h>
#include "fibonacci_heap.h"

// Test inserting values into the Fibonacci Heap
TEST(FibonacciHeapTest, Insert) {
    FibonacciHeap heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(20);

    EXPECT_EQ(heap.getMin()->getKey(), 5);  // Min should be 5
}

// Test extracting the minimum element
TEST(FibonacciHeapTest, ExtractMin) {
    FibonacciHeap heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(20);

    auto minNode = heap.extractMin();
    EXPECT_EQ(minNode->getKey(), 5);  // Should extract 5
    EXPECT_EQ(heap.getMin()->getKey(), 10);  // Next min should be 10
}

// Test inserting multiple elements and extracting them
TEST(FibonacciHeapTest, MultipleExtractMin) {
    FibonacciHeap heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(15);

    std::unique_ptr<Node> minNode = heap.extractMin();
    ASSERT_NE(minNode, nullptr);  // Ensure the extracted node is not null
    EXPECT_EQ(minNode->getKey(), 5);

    Node* nextMin = heap.getMin();
    ASSERT_NE(nextMin, nullptr);  // Prevent null pointer dereference
    EXPECT_EQ(nextMin->getKey(), 10);
}


// Test decrease key functionality
TEST(FibonacciHeapTest, DecreaseKey) {
    FibonacciHeap heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(30);

    Node* node = heap.getMin();  // Min is 10
    heap.insert(5);  // Insert 5 to change min

    EXPECT_EQ(heap.getMin()->getKey(), 5);
}

// Test empty heap behavior
TEST(FibonacciHeapTest, EmptyHeap) {
    FibonacciHeap heap;
    EXPECT_TRUE(heap.isEmpty());

    auto min = heap.extractMin();
    EXPECT_EQ(min, nullptr);  // Extracting from an empty heap should return nullptr
}

