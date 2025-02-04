#include <iostream>
#include <vector>

using namespace std;

struct FibonacciHeapNode {
public:
  int key;
  FibonacciHeapNode *parent;
  FibonacciHeapNode *child;
  FibonacciHeapNode *left;
  FibonacciHeapNode *right;
  int degree;
  bool mark;

  FibonacciHeapNode(int k)
      : key(k), parent(nullptr), child(nullptr), left(this), right(this),
        degree(0), mark(false) {}
};

class FibonacciHeap {
private:
  FibonacciHeapNode *min_;
  int count_;

  void addRoot(FibonacciHeapNode *node) {
    if (!min_) {
      min_ = node;
      node->left = node;
      node->right = node;
      return;
    }

    node->left = min_;
    node->right = min_->right;
    min_->right->left = node;
    min_->right = node;
    node->parent = nullptr;
  }

  void consolidate() {
    if (!min_)
      return;

    vector<FibonacciHeapNode *> A(count_ + 1, nullptr);
    vector<FibonacciHeapNode *> nodes;

    FibonacciHeapNode *current = min_;
    do {
      nodes.push_back(current);
      current = current->right;
    } while (current != min_);

    for (FibonacciHeapNode *w : nodes) {
      int d = w->degree;
      while (A[d] != nullptr) {
        FibonacciHeapNode *y = A[d];
        if (w->key > y->key) {
          swap(w, y);
        }
        link(y, w);
        A[d] = nullptr;
        d++;
      }
      if (d >= A.size())
        A.resize(d + 1, nullptr);
      A[d] = w;
    }

    min_ = nullptr;
    for (FibonacciHeapNode *a : A) {
      if (a != nullptr) {
        if (!min_ || a->key < min_->key) {
          min_ = a;
        }
      }
    }
  }

  static void link(FibonacciHeapNode *y, FibonacciHeapNode *x) {
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;

    if (!x->child) {
      x->child = y;
      y->left = y;
      y->right = y;
    } else {
      y->left = x->child;
      y->right = x->child->right;
      x->child->right->left = y;
      x->child->right = y;
    }
    x->degree++;
    y->mark = false;
  }

  void cut(FibonacciHeapNode *x, FibonacciHeapNode *y) {
    if (x->right == x) {
      y->child = nullptr;
    } else {
      y->child = x->right;
      x->left->right = x->right;
      x->right->left = x->left;
    }
    y->degree--;

    addRoot(x);
    x->mark = false;
  }

  void cascadingCut(FibonacciHeapNode *y) {
    if (FibonacciHeapNode *z = y->parent) {
      if (!y->mark) {
        y->mark = true;
      } else {
        cut(y, z);
        cascadingCut(z);
      }
    }
  }

public:
  FibonacciHeap() : min_(nullptr), count_(0) {}

  ~FibonacciHeap() { delete[] min_; }

  FibonacciHeapNode *insert(int key) {
    FibonacciHeapNode *node = new FibonacciHeapNode(key);
    if (!min_) {
      min_ = node;
    } else {
      node->left = min_;
      node->right = min_->right;
      min_->right->left = node;
      min_->right = node;
      if (node->key < min_->key) {
        min_ = node;
      }
    }
    count_++;
    return node;
  }

  FibonacciHeapNode *extractMin() {
    if (!min_)
      return nullptr;

    FibonacciHeapNode *z = min_;
    if (z->child) {
      FibonacciHeapNode *child = z->child;
      do {
        FibonacciHeapNode *next = child->right;
        addRoot(child);
        child->parent = nullptr;
        child = next;
      } while (child != z->child);
      z->child = nullptr;
    }

    if (z == z->right) {
      min_ = nullptr;
    } else {
      z->left->right = z->right;
      z->right->left = z->left;
      min_ = z->right;
      consolidate();
    }

    count_--;
    if (count_ == 0)
      min_ = nullptr;
    return z;
  }

  void decreaseKey(FibonacciHeapNode *x, int k) {
    if (k > x->key) {
      throw invalid_argument("New key is greater than current key");
    }
    x->key = k;
    if (FibonacciHeapNode *y = x->parent; y && x->key < y->key) {
      cut(x, y);
      cascadingCut(y);
    }
    if (!min_ || x->key < min_->key) {
      min_ = x;
    }
  }

  bool isEmpty() const { return min_ == nullptr; }
  FibonacciHeapNode *getMin() const { return min_; }
};

int main() {
  FibonacciHeap heap;

  // Тест 1: Вставка и извлечение минимума
  heap.insert(3);
  heap.insert(2);
  heap.insert(1);
  FibonacciHeapNode *min = heap.extractMin();
  cout << "Extracted min: " << (min ? min->key : -1) << endl; // Должно быть 1

  FibonacciHeapNode *min2 = heap.extractMin();
  cout << "Extracted min: " << (min2 ? min2->key : -1) << endl; // Должно быть 1

  // Тест 2: Уменьшение ключа
  FibonacciHeapNode *node = heap.insert(5);
  heap.insert(4);
  heap.decreaseKey(node, 0);
  min = heap.extractMin();
  cout << "Extracted min after decreaseKey: " << (min ? min->key : -1)
       << endl; // Должно быть 0

  // Тест 3: Множественное извлечение
  for (int i = 0; i < 10; ++i) {
    heap.insert(i);
  }
  while (!heap.isEmpty()) {
    heap.extractMin();
  }
  cout << "Heap is empty: " << heap.isEmpty() << endl; // Должно быть 1

  return 0;
}