#include "fibonacci_heap.h"
#include <vector>
#include <cmath>

// ----------------------- Node Implementation -----------------------

Node::Node(const int k) : key(k), parent(nullptr), degree(0), mark(false) {
}

Node::~Node() {
  children.clear();
}

int Node::getKey() const {
  return key;
}

void Node::setKey(int k) {
  key = k;
}

// ----------------------- ListNode Implementation -----------------------

ListNode::ListNode(Node *value) : data(value), next(nullptr), prev(nullptr) {
}

ListNode::~ListNode() {
  if (data) {
    delete data;
    data = nullptr;
  };
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
  if (!head) {
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

void DoublyLinkedList::deleteNode(ListNode *node) {
  if (!node || !node->data) return;

  for (Node* child : node->data->children) {
    if (child) {
      child->parent = nullptr;
      append(child);
    }
  }

  if (node == head && node == tail) {
    head = nullptr;
    tail = nullptr;
    minNode = nullptr;
  } else if (node == head) {
    head = head->next;
    if (head) head->prev = nullptr;
  } else if (node == tail) {
    tail = tail->prev;
    if (tail) tail->next = nullptr;
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

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

  delete node;
}

ListNode *DoublyLinkedList::getMinNode() const {
  return minNode;
}

bool DoublyLinkedList::isEmpty() const {
  return head == nullptr;
}

// ----------------------- FibonacciHeap Implementation -----------------------

FibonacciHeap::FibonacciHeap() : count_(0) {
}

FibonacciHeap::~FibonacciHeap() = default;

void FibonacciHeap::insert(const int key) {
  Node* newNode = new Node(key);
  list_.append(newNode);
  count_++;
}

std::unique_ptr<Node> FibonacciHeap::extractMin() {
  ListNode* min = list_.getMinNode();
  if (!min) return nullptr;

  std::unique_ptr<Node> ret(min->data);
  min->data = nullptr;

  count_--;
  list_.deleteNode(min);
  consolidate();
  return ret;
}

Node *FibonacciHeap::getMin() const {
  ListNode* minNode = list_.getMinNode();
  return (minNode && minNode->data) ? minNode->data : nullptr;
}

bool FibonacciHeap::isEmpty() const {
  return count_ == 0;
}

void FibonacciHeap::consolidate() {
  if (count_ <= 1) { return; }
  int maxDegree = static_cast<int>(log2(count_));
  std::vector<ListNode*> A(maxDegree + 1, nullptr);

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
    int d = x->data->degree;

    // Merge trees of the same degree
    while (A[d]) {
      ListNode* y = A[d];
      if (y->data->key < x->data->key) {
        std::swap(y, x);
      }
      link(y, x);
      A[d] = nullptr;
      d++;
    }
    A[d] = x;
  }

  // Rebuild the root list and find the new minNode
  list_.head = nullptr;
  list_.tail = nullptr;
  list_.minNode = nullptr;

  for (ListNode* node: A) {
    if (node) {
      if (!list_.head) {
        list_.head = node;
        list_.tail = node;
        list_.minNode = node;
        node->next = nullptr;
        node->prev = nullptr;
      } else {
        list_.tail->next = node;
        node->prev = list_.tail;
        list_.tail = node;
        if (node->data->key < list_.minNode->data->key) {
          list_.minNode = node;
        }
      }
    }
  }
}

void FibonacciHeap::link(ListNode* y, ListNode* x) {
  // Remove y from the root list without deleting it
  if (y == list_.head) {
    list_.head = y->next;
    if (list_.head) list_.head->prev = nullptr;
  } else if (y == list_.tail) {
    list_.tail = y->prev;
    if (list_.tail) list_.tail->next = nullptr;
  } else {
    if (y->prev) y->prev->next = y->next;
    if (y->next) y->next->prev = y->prev;
  }

  y->data->parent = x->data; // Set parent
  x->data->children.push_back(y->data); // Add as child
  x->data->degree++; // Increase degree

  y->next = nullptr; // Disconnect y from the list structure
  y->prev = nullptr;
}
