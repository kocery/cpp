#include "fibonacci_heap.h"
#include <vector>
#include <cmath>

// ----------------------- Node Implementation -----------------------

Node::Node(const int k) : key(k), parent(nullptr), degree(1), mark(false) {
}

Node::~Node() {
  for (const Node* child : children) {
    delete child;
  }
  children.clear();
}

int Node::getKey() const {
  return key;
}

int Node::getDegree() const {
  return degree;
}

void Node::setKey(const int k) {
  key = k;
}

void Node::link(Node* x, Node* y) {
  y->parent = x;
  x->children.push_back(y);
  x->degree += y->degree;
}

// ----------------------- ListNode Implementation -----------------------

ListNode::ListNode(Node *value) : data(value), next(nullptr), prev(nullptr) {
}

ListNode::~ListNode() = default;

Node* ListNode::getData() const {
  return data;
}

// ----------------------- DoublyLinkedList Implementation -----------------------

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), minNode(nullptr) {
}

DoublyLinkedList::~DoublyLinkedList() {
  const ListNode* current = head;
  while (current) {
    const ListNode* next = current->next;
    delete current->data;
    delete current;
    current = next;
  }
}

void DoublyLinkedList::append(Node* node) {
  const auto newNode = new ListNode(node);
  if (!head || !tail || !minNode) {
    head = newNode;
    tail = newNode;
    minNode = newNode;
  } else {
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;

    // Update minNode if necessary
    if (node->key < minNode->data->key) {
      minNode = newNode;
    }
  }
}

void DoublyLinkedList::deleteNode(const ListNode* node) {
  if (!node || !node->data) {return;}

  for (Node* child : node->data->children) {
    if (child) {
      child->parent = nullptr;
      append(child);
    }
  }
  node->data->children.clear();

  extractNode(node);

  if (node == minNode) {
    minNode = head;
    ListNode* current = head;
    while (current) {
      if (current->data->key < minNode->data->key) {
        minNode = current;
      }
      current = current->next;
    }
  }

  delete node->data;
  delete node;
}

ListNode *DoublyLinkedList::getMinNode() const {
  return minNode;
}

bool DoublyLinkedList::isEmpty() const {
  return head == nullptr;
}

void DoublyLinkedList::extractNode(const ListNode* node) {
  if (node == head) {
    head = node->next;
    if (head) {head->prev = nullptr;}
  }
  if (node == tail) {
    tail = node->prev;
    if (tail) {tail->next = nullptr;}
  } else {
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
  }
}

void DoublyLinkedList::clear() {
  head = nullptr;
  tail = nullptr;
  minNode = nullptr;
}

// ----------------------- FibonacciHeap Implementation -----------------------

FibonacciHeap::FibonacciHeap() : count_(0) {
}

FibonacciHeap::~FibonacciHeap() = default;

void FibonacciHeap::insert(const int key) {
  list_.append(new Node(key));
  count_++;
}

std::optional<int> FibonacciHeap::extractMin() {
  if (isEmpty()) return std::nullopt;

  const ListNode* min = list_.getMinNode();
  int out = min->data->getKey();

  count_--;
  list_.deleteNode(min);
  consolidate();
  return out;
}

bool FibonacciHeap::isEmpty() const {
  return count_ == 0;
}

Node* FibonacciHeap::getMin() const {
  const ListNode* minNode = list_.getMinNode();
  return (minNode && minNode->data) ? minNode->data : nullptr;
}

void FibonacciHeap::consolidate() {
  if (count_ <= 1) { return; }
  std::vector<ListNode*> A(count_ + 1, nullptr);

  ListNode* current = list_.head;

  while (current && current->data) {
    int d = current->data->getDegree() - 1;

    // Merge trees of the same degree
    while (A[d]) {
      ListNode* y = A[d];
      if (y->data->getKey() < current->data->getKey()) {
        std::swap(current, y);
      }
      link(y, current);
      A[d] = nullptr;
      d++;
    }
    A[d] = current;
    current = current->next;
  }
  A.clear();
}

void FibonacciHeap::link(ListNode* y, const ListNode* x) {
  // Remove y from the root list without deleting it and link two nodes
  list_.extractNode(y);
  Node::link(x->data, y->data);
  y->data = nullptr;
  delete y;
}
