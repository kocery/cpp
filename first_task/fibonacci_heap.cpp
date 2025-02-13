#include "fibonacci_heap.h"
#include <vector>
#include <cmath>

// ----------------------- Node Implementation -----------------------

Node::Node(const int k) : key(k), parent(nullptr), degree(0), mark(false) {
}

Node::~Node() {
   clearChildren();
}

int Node::getKey() const {
  return key;
}

int Node::getDegree() const {
  return degree;
}

void Node::setKey(int k) {
  key = k;
}

void Node::link(Node *x, Node *y) {
  y->parent = x; // Set parent
  x->children.push_back(y); // Add as child
  x->degree++; // Increase degree
}


void Node::clearChildren() {
  for (Node* child : children) {
    delete child;
  }
  children.clear();
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
  ListNode* current = head;
  while (current) {
    ListNode* next = current->next;
    delete current;
    current = next;
  }
}

void DoublyLinkedList::append(Node* node) {
  ListNode* newNode = new ListNode(node);
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

void DoublyLinkedList::deleteNode(ListNode* node) {
  if (!node || !node->data) return;

  for (Node* child : node->data->children) {
    if (child) {
      child->parent = nullptr;
      append(child);
    }
  }

  extractNode(*node);

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

Node& DoublyLinkedList::extractNode(ListNode& node) {
  if (&node == head) {
    head = node.next;
    if (head) head->prev = nullptr;
  } else if (&node == tail) {
    tail = node.prev;
    if (tail) tail->next = nullptr;
  } else {
    if (node.prev) node.prev->next = node.next;
    if (node.next) node.next->prev = node.prev;
  }
  return *node.data;
}

void DoublyLinkedList::clear() {
  head = nullptr;
  tail = nullptr;
  minNode = nullptr;
}

// ----------------------- FibonacciHeap Implementation -----------------------

FibonacciHeap::FibonacciHeap() : count_(0) {
}

FibonacciHeap::~FibonacciHeap() {
  ListNode* current = list_.head;
  while (current) {
    delete current->data;
    current = current->next;
  }
};

void FibonacciHeap::insert(const int key) {
  Node* newNode = new Node(key);
  list_.append(newNode);
  count_++;
}

std::optional<int> FibonacciHeap::extractMin() {
  if (isEmpty()) return std::nullopt;

  ListNode* min = list_.getMinNode();
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
  ListNode* minNode = list_.getMinNode();
  return (minNode && minNode->data) ? minNode->data : nullptr;
}

void FibonacciHeap::consolidate() {
  if (count_ <= 1) { return; }
  int maxDegree = static_cast<int>(log2(count_));
  std::vector<ListNode*> A(2*maxDegree + 1, nullptr);

  ListNode* current = list_.head;
  std::vector<ListNode*> roots;

  // Store all root nodes in a separate vector to avoid modifying the list while iterating
  while (current) {
    roots.push_back(current);
    current = current->next;
  }

  for (ListNode* node: roots) {
    if (!node || !node->data) continue;

    ListNode* x = node;
    int d = x->data->getDegree();

    // Merge trees of the same degree
    while (A[d]) {
      ListNode* y = A[d];
      if (y->data->getKey() < x->data->getKey()) {
        std::swap(y, x);
      }
      link(y, x);
      A[d] = nullptr;
      d++;
    }
    A[d] = x;
  }

  // Rebuild the root list and find the new minNode
  list_.clear();

  for (ListNode* node: A) {
    if (node) {
      list_.append(node->data);
    }
  }
}

void FibonacciHeap::link(ListNode* y, ListNode* x) {
  // Remove y from the root list without deleting it and link two nodes
  list_.extractNode(*y);
  Node::link(x->data, y->data);
}
