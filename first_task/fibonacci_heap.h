#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <list>
#include <optional>

class Node {
public:
  Node(int k);
  ~Node();

  int getKey() const;
  int getDegree() const;
  void setKey(int k);
  static void link(Node* x, Node* y);

private:
  int key;
  Node* parent;
  std::list<Node*> children;
  int degree;
  bool mark;

  friend class DoublyLinkedList;
};

class ListNode {
public:
  ListNode(Node* value);
  ~ListNode();
  Node* getData() const;

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
  void deleteNode(const ListNode* node);
  ListNode* getMinNode() const;
  bool isEmpty() const;
  void extractNode(const ListNode* node);

private:
  ListNode* head;
  ListNode* tail;
  ListNode* minNode;

  void clear();

  friend class FibonacciHeap;
};

class FibonacciHeap {
public:
  FibonacciHeap();
  ~FibonacciHeap();

  void insert(int key);
  std::optional<int> extractMin();
  bool isEmpty() const;
  Node* getMin() const;

private:
  void consolidate();
  void link(ListNode* y, const ListNode* x);

  DoublyLinkedList list_;
  int count_;
};

#endif
