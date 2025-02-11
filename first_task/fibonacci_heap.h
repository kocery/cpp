#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <list>
#include <memory>

class Node {
public:
  Node(int k);
  ~Node();

  int getKey() const;
  void setKey(int k);

private:
  int key;
  Node* parent;
  std::list<Node*> children;
  int degree;
  bool mark;

  friend class DoublyLinkedList;
  friend class FibonacciHeap;
};

class ListNode {
public:
  ListNode(Node* value);
  ~ListNode();

private:
  Node* data;
  ListNode* next;
  ListNode* prev;

  friend class DoublyLinkedList;
  friend class FibonacciHeap;
};

class DoublyLinkedList {
public:
  DoublyLinkedList();
  ~DoublyLinkedList();

  void append(Node* node);
  void deleteNode(ListNode* node);
  ListNode* getMinNode() const;
  bool isEmpty() const;

private:
  ListNode* head;
  ListNode* tail;
  ListNode* minNode;

  friend class FibonacciHeap;
};

class FibonacciHeap {
public:
  FibonacciHeap();
  ~FibonacciHeap();

  void insert(int key);
  std::unique_ptr<Node> extractMin();
  bool isEmpty() const;
  Node* getMin() const;

private:
  void consolidate();
  void link(ListNode* y, ListNode* x);

  DoublyLinkedList list_;
  int count_;
};

#endif
