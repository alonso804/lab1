#pragma once

#include "SpatialBase.h"

#include <iostream>
#include <stack>
#include <utility>


using namespace std;

namespace utec {
namespace spatial {
template <typename Point>
class RangeBST;

template <typename Point>
class Node {
 private:
  int key;
  Point data;
  Node<Point>* left;
  Node<Point>* right;

  Node(int, Point);
  Node(int);

  void killSelf();

  template <class>
  friend class RangeBST;
};

template <typename Point>
Node<Point>::Node(int key, Point data) {
  this->key = key;
  this->data = data;
  this->left = nullptr;
  this->right = nullptr;
}

template <typename Point>
Node<Point>::Node(int key) {
  this->key = key;
  this->left = nullptr;
  this->right = nullptr;
}

template <typename Point>
void Node<Point>::killSelf() {
  if (left) {
    left->killSelf();
  }

  if (right) {
    right->killSelf();
  }

  delete this;
}

/**
 * RangeBST implementation
 */
template <typename Point>
class RangeBST : public SpatialBase<Point> {
 private:
  Node<Point>* root;
  int nodes;

  //------------Extra------------------
  bool find(int, Node<Point>**&);

  void deletion_for_two_branches(Node<Point>**&, Node<Point>**&);

  void PreOrder(Node<Point>*&);

  void InOrder(Node<Point>*&);

  void PostOrder(Node<Point>*&);

  int height_from_node(Node<Point>*&);

  void rightRotation(Node<Point>*& x) {
    auto y = x->left;
    auto rightY = y->right;

    y->right = x;
    x->left = rightY;
  }

  void leftRotation(Node<Point>*& x) {
    auto y = x->right;
    auto leftY = y->left;

    y->left = x;
    x->right = leftY;
  }

  int getBalanceHeight(Node<Point>*& temp) {
    if (empty()) {
      return 1;
    }

    return height_from_node(temp->left) - height_from_node(temp->right);
  }

 public:
  RangeBST();
  ~RangeBST();

  bool insert(int, Point);
  bool remove(int);
  bool hasKey(int);
  Point& operator[](int);
  int size();
  int height();
  bool empty() const;
  void traversePreOrder();
  void traverseInOrder();
  void traversePostOrder();
  void insert(const Point& new_point) override {}

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override { return Point({0}); }
  std::vector<Point> range(const Point& min, const Point& max) override {
    return {};
  };
};

template <typename Point>
RangeBST<Point>::RangeBST() {
  this->root = nullptr;
  this->nodes = 0;
}

template <typename Point>
RangeBST<Point>::~RangeBST() {
  if (root) {
    root->killSelf();
  }
}

template <typename Point>
bool RangeBST<Point>::insert(int key, Point data) {
  Node<Point>* new_node = new Node<Point>(key, data);
  Node<Point>** pointer = &root;

  bool result = false;
  bool finder = find(key, pointer);

  if (finder) {
    (*pointer)->data = data;
    return false;
  } else {
    (*pointer) = new_node;
    result = true;
    ++nodes;
  }

  // TODO: complete rotations
  int height = this->getBalanceHeight(*pointer);
  cout << height << endl;

  return result;
}

template <typename Point>
bool RangeBST<Point>::remove(int key) {
  if (empty()) {
    cerr << "RangeBST is empty\n";
    return false;
  }

  Node<Point>** pointer = &root;

  bool result = false;

  if (find(key, pointer)) {
    if (!((*pointer)->left)) {
      auto to_delete = *pointer;
      *pointer = ((*pointer)->right);

      delete to_delete;
    } else if (!((*pointer)->right)) {
      auto to_delete = *pointer;
      *pointer = ((*pointer)->left);

      delete to_delete;
    } else {
      Node<Point>** temp = pointer;
      deletion_for_two_branches(pointer, temp);
    }

    --nodes;
    result = true;
  }

  return result;
}

template <typename Point>
bool RangeBST<Point>::hasKey(int key) {
  Node<Point>** pointer = &root;

  return (find(key, pointer));
}

template <typename Point>
Point& RangeBST<Point>::operator[](int index) {  // index = key
  Node<Point>** pointer = &root;
  bool finder = find(index, pointer);

  if (!finder) {
    Node<Point>* new_node = new Node<Point>(index);
    (*pointer) = new_node;
    ++nodes;
  }

  return (*pointer)->data;
}

template <typename Point>
int RangeBST<Point>::size() {
  return nodes;
}

template <typename Point>
int RangeBST<Point>::height() {
  auto temp = root;

  return height_from_node(temp);
}

template <typename Point>
bool RangeBST<Point>::empty() const {
  return (!root);
}

template <typename Point>
void RangeBST<Point>::traversePreOrder() {
  if (root) {
    auto temp = root;
    PreOrder(temp);
  }
}

template <typename Point>
void RangeBST<Point>::traverseInOrder() {
  if (root) {
    auto temp = root;
    InOrder(temp);
  }
}

template <typename Point>
void RangeBST<Point>::traversePostOrder() {
  if (root) {
    auto temp = root;
    PostOrder(temp);
  }
}

//---------------------------Extra------------------------------
template <typename Point>
bool RangeBST<Point>::find(int key, Node<Point>**& pointer) {
  while ((*pointer) != nullptr) {
    if (((*pointer)->key) == key) {
      return true;
    }

    if (key < (*pointer)->key) {
      pointer = &((*pointer)->left);
    } else {
      pointer = &((*pointer)->right);
    }
  }

  return false;
}

template <typename Point>
void RangeBST<Point>::deletion_for_two_branches(Node<Point>**& pointer,
                                                Node<Point>**& temp) {
  temp = &((*temp)->right);

  while ((*temp)->left != nullptr) {
    temp = &((*temp)->left);
  }

  swap((*pointer)->key, (*temp)->key);
  swap((*pointer)->data, (*temp)->data);

  auto to_delete = *temp;
  *temp = (*temp)->right;

  delete to_delete;
}

template <typename Point>
void RangeBST<Point>::PreOrder(Node<Point>*& temp) {
  if (temp) {
    cout << temp->key << '\t' << temp->data << endl;

    if (temp->left) {
      PreOrder(temp->left);
    }

    if (temp->right) {
      PreOrder(temp->right);
    }
  }
}

template <typename Point>
void RangeBST<Point>::InOrder(Node<Point>*& temp) {
  if (temp) {
    if (temp->left) {
      InOrder(temp->left);
    }

    cout << temp->key << '\t' << temp->data << endl;

    if (temp->right) {
      InOrder(temp->right);
    }
  }
}

template <typename Point>
void RangeBST<Point>::PostOrder(Node<Point>*& temp) {
  if (temp) {
    if (temp->left) {
      PostOrder(temp->left);
    }

    if (temp->right) {
      PostOrder(temp->right);
    }

    cout << temp->key << '\t' << temp->data << endl;
  }
}

template <typename Point>
int RangeBST<Point>::height_from_node(Node<Point>*& temp) {
  if (root) {
    int left_height = -1, right_height = -1;
    if (temp->left) {
      left_height = height_from_node(temp->left);
    }

    if (temp->right) {
      right_height = height_from_node(temp->right);
    }

    return max(left_height, right_height) + 1;
  }
  return (-1);
}

}  // namespace spatial
}  // namespace utec
