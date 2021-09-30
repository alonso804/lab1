#pragma once

#include "SpatialBase.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <vector>

using namespace std;

namespace utec {
  namespace spatial {

  template <typename Point>
  class RangeBST; 

  template <typename Point>
  class Node {
  public:
      Point data;
      Node<Point>* left;
      Node<Point>* right;
  private:
      Node(Point data) {
          this->data = data;
          this->left = nullptr;
          this->right = nullptr;
      }
      void killSelf() {
          if(left){
              left->killSelf();
          }
          if(right){
              right->killSelf();
          }

          delete this;
      }
      template<class>
      friend class RangeBST; 
  };

    template <typename Point>
    class RangeBST : public SpatialBase<Point> {
      private:
        Node<Point>* root;
        int nodes;

        bool find(Point key, Node<Point>**& pointer, stack<Node<Point>**>& parents) {
          while((*pointer) != nullptr) {
            if(((*pointer)->data) == key){
              return true;
            }
            parents.push(pointer);
            if(key < (*pointer)->data){
              pointer = &((*pointer)->left);
            } else {
              pointer = &((*pointer)->right);
            }
          }
          return false;
        }

        int height_from_node(Node<Point>*& temp) {
          if(temp){
            int left_height = -1, right_height = -1;
            if(temp->left){
              left_height = height_from_node(temp->left);
            }
            if(temp->right){
              right_height = height_from_node(temp->right);
            }
            return std::max(left_height, right_height) + 1;
          }
          return (-1);
        }

        Node<Point>* rightRotation(Node<Point>*& pointer) {
          auto y = (pointer)->left;
          auto rightY = y->right;
          y->right = (pointer);
          (pointer)->left = rightY;
          return y;
        }

        Node<Point>* leftRotation(Node<Point>*& pointer) {
          auto y = (pointer)->right;
          auto leftY = y->left;
          y->left = (pointer);
          (pointer)->right = leftY;
          return y;
        }

        void rotate(stack<Node<Point>**>& parents, const Point& data) {
          while (!parents.empty()) {
            auto node = parents.top();
            int balanceHeight = getBalanceHeight((*node));
            bool hasRotate = false;
            Node<Point>* newNode = nullptr;
            if (balanceHeight > 1 && data < (*node)->left->data) {
                newNode = rightRotation(*node);
                hasRotate = true;
            } else if (balanceHeight < -1 && (*node)->right->data < data) {
                newNode = leftRotation(*node);
                hasRotate = true;
            } else if (balanceHeight > 1 && (*node)->left->data < data) {
                (*node)->left = leftRotation(((*node)->left));
                newNode = rightRotation(*node);
                hasRotate = true;
            } else if (balanceHeight < -1 && data < (*node)->right->data) {
                (*node)->right = rightRotation(((*node)->right));
                newNode = leftRotation(*node);
                hasRotate = true;
            }
            if (hasRotate) {
                (*node) = newNode;
            }
            parents.pop();
          }
        }

        int getBalanceHeight(Node<Point>*& temp) {
          auto leftHeight = height_from_node(temp->left);
          auto rightHeight = height_from_node(temp->right);
          return leftHeight - rightHeight;
        }

        void rangeQuery(const Point& min, const Point& max, Node<Point>* node, vector<Point>& result){
          if (!node) {
              return;
          }
          if (min.get(0) <= node->data.get(0) && node->data.get(0) <= max.get(0)) {
              rangeQuery(min, max, node->left, result);
              result.push_back(node->data);
              rangeQuery(min, max, node->right, result);
          }
          else if (node->data.get(0) < min.get(0)) {
              rangeQuery(min,max, node->right, result);
          }
          else if (max.get(0) < node->data.get(0)) {
              rangeQuery(min, max, node->left, result);
          }
        }
      public:
        RangeBST() {
          this->root = nullptr;
          this->nodes = 0;
        }

        ~RangeBST() {
          if(root){
            root->killSelf();
          }
        }

        int size() {
          return nodes;
        }

        int height() {
          auto temp = root;
          return height_from_node(temp);
        }

        bool empty() const {
          return (!root);
        }

        void insert(const Point& new_point) {
          Node<Point>* new_node = new Node<Point>(new_point);
          Node<Point>** pointer = &root;
          stack<Node<Point>**> parents;

          bool finder = find(new_point, pointer, parents);

          if (finder) {
            (*pointer)->data = new_point;
          } else {
            (*pointer) = new_node;
            ++this->nodes;
          }

          if (height() > 1) {
            parents.pop();
            rotate(parents, new_point);
          }
        }

        Point nearest_neighbor(const Point& reference) override { return Point({0}); }

        vector<Point> range(const Point& min, const Point& max) override {
          vector<Point> result;
          rangeQuery(min, max, root, result);
          return result;
        };
    };
  }  
}  
