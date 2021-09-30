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
    class RangeTree1D; 

    template <typename Point>
    class Node1 {
        Point data;
        Node1<Point>* left;
        Node1<Point>* right;
        int height;

    private:
        Node1(Point data) {
            this->data = data;
            this->left = nullptr;
            this->right = nullptr;
            this->height = 0;
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
        friend class RangeTree1D; 
    };

    template <typename Point> 
    class RangeTree1D {
        Node1<Point>* root;
        int nodes;

        bool find(Point key, Node1<Point>**& pointer, stack<Node1<Point>**>& parents) {
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

        bool findRange(Point key, Node1<Point>**& pointer, stack<Node1<Point>**>& parents) {
            while((*pointer) != nullptr) {
                if(((*pointer)->data) == key && (*pointer)->height == 0){
                    return true;
                }

                parents.push(pointer);

                if(key < (*pointer)->data || key == (*pointer)->data) {
                    pointer = &((*pointer)->left);
                } else {
                    pointer = &((*pointer)->right);
                }
            }
            
            return false;
        }

        int nodeHeight(Node1<Point>*& node) {
          if (node) {
    /*
     *        int left_height = -1, right_height = -1;
     *        if(node->left){
     *            left_height = nodeHeight(node->left);
     *        }
     *
     *        if(node->right){
     *            right_height = nodeHeight(node->right);
     *        }
     *
     *        return max(left_height, right_height) + 1;
     */
            return node->height;
          }
          return (-1);
        }

        Node1<Point>* rightRotation(Node1<Point>*& pointer) {
            auto y = (pointer)->left;
            auto rightY = y->right;

            y->right = (pointer);
            (pointer)->left = rightY;

            pointer->height = 1 + max(nodeHeight(pointer->left), nodeHeight(pointer->right));
            y->height = 1 + max(nodeHeight(y->left), nodeHeight(y->right));

            return y;
        }

        Node1<Point>* leftRotation(Node1<Point>*& pointer) {
            auto y = (pointer)->right;
            auto leftY = y->left;

            y->left = (pointer);
            (pointer)->right = leftY;

            pointer->height = 1 + max(nodeHeight(pointer->left), nodeHeight(pointer->right));
            y->height = 1 + max(nodeHeight(y->left), nodeHeight(y->right));

            return y;
        }

        void rotate(stack<Node1<Point>**>& parents, const Point& data) {
            while (!parents.empty()) {
                auto node = parents.top();

                (*node)->height = 1 + max(nodeHeight((*node)->left), nodeHeight((*node)->right));

                int balanceHeight = getBalanceHeight((*node));

                bool hasRotate = false;
                Node1<Point>* newNode1 = nullptr;

                if (balanceHeight > 1 && (data < (*node)->left->data || (*node)->left->data == data)) {
                    newNode1 = rightRotation(*node);
                    hasRotate = true;
                } else if (balanceHeight < -1 && ((*node)->right->data < data || (*node)->right->data == data)) {
                    newNode1 = leftRotation(*node);
                    hasRotate = true;
                } else if (balanceHeight > 1 && ((*node)->left->data < data)) {
                    (*node)->left = leftRotation(((*node)->left));
                    newNode1 = rightRotation(*node);
                    hasRotate = true;
                } else if (balanceHeight < -1 && data < (*node)->right->data) {
                    (*node)->right = rightRotation(((*node)->right));
                    newNode1 = leftRotation(*node);
                    hasRotate = true;
                }

                if (hasRotate) {
                    (*node) = newNode1;
                }

                parents.pop();
            }
        }

        int getBalanceHeight(Node1<Point>*& temp) {
          auto leftHeight = nodeHeight(temp->left);
          auto rightHeight = nodeHeight(temp->right);

          return leftHeight - rightHeight;
        }

        void InOrder(Node1<Point>*& temp){
            if(temp){
                if(temp->left){
                    InOrder(temp->left);
                }

                cout << temp->data.get(0) << " => " << nodeHeight(temp) << endl;

                if(temp->right){
                    InOrder(temp->right);
                }
            }
        }

        void rangeQuery(const Point& min, const Point& max, Node1<Point>* node, vector<Node1<Point>*>& result){
            Node1<Point>** pointer = &root;
            stack<Node1<Point>**> parents;

            bool finder = findRange(min, pointer, parents);

            if (finder) {
                result.push_back((*pointer));
            }

            parents.pop();

            //while ((*(parents.top()))->data < max || (*(parents.top()))->data == max) {
            while ((*(parents.top()))->data < max) {
                if ((*(parents.top()))->height == 0) {
                    result.push_back((*(parents.top())));
                }

                if((*(parents.top()))->right){
                    auto temp = &(*(parents.top()))->right;
                    parents.pop();
                    parents.push(temp);

                    while((*temp)->left){
                        parents.push(&(*temp)->left);    
                        temp = &((*temp)->left);
                    }
                } else {
                    parents.pop();
                }
            }
        }

    public:
        RangeTree1D() {
            this->root = nullptr;
            this->nodes = 0;
        }

        ~RangeTree1D() {
            if(root){
                root->killSelf();
            }
        }

        int size() {
            return nodes;
        }

        int height() {
            auto temp = root;

            return nodeHeight(temp);
        }

        bool empty() const {
            return (!root);
        }

        void print(Point index) {
            Node1<Point>** pointer = &root;
            stack<Node1<Point>**> parents;
            bool finder = find(index, pointer, parents);

            if(!finder){
                Node1<Point>* new_node = new Node1<Point>(index);
                (*pointer) = new_node;
                ++nodes;
            }

            cout << "Data: " << (*pointer)->data.get(0) << endl;
            cout << "Height: " << nodeHeight(*pointer) << endl;
        }

        void insert(const Point& new_point) {
          Node1<Point>* new_node = new Node1<Point>(new_point);
          Node1<Point>** pointer = &root;
          stack<Node1<Point>**> parents;

          bool finder = find(new_point, pointer, parents);

          if (finder) {
            (*pointer)->data = new_point;
          } else {
            (*pointer) = new_node;
            ++this->nodes;
          }

          if (this->nodes > 1) {
            auto parent = parents.top();
            (*parent)->height = 1 + max(nodeHeight((*parent)->left),
                                        nodeHeight((*parent)->right));

            auto parentData = (*parent)->data;
            if (!(new_point < (*parent)->data)) {
              new_node = new Node1<Point>(parentData);
            } else {
              new_node = new Node1<Point>(new_point);
            }

            (*pointer)->left = new_node;
            (*pointer)->height = 1 + max(nodeHeight((*pointer)->left),
                                         nodeHeight((*pointer)->right));

            if (!(new_point < (*parent)->data)) {
              rotate(parents, parentData);
            } else {
              rotate(parents, new_point);
            }
          }
        }

        void traverseInOrder(){
            if(root){
                auto temp = root;
                InOrder(temp);
            } 
        }

        vector<Point> range(const Point& min, const Point& max) {
            vector<Node1<Point>*> result;
            rangeQuery(min, max, root, result);

            vector<Point> pointers;

            for (auto& point : result) {
                pointers.push_back((*point).data);
            }

            return pointers;
        };

        Point nearest_neighbor(const Point& reference) { return Point({0}); }
    };
  }  // namespace spatial
}  // namespace utec
