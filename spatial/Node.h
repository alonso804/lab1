#ifndef NODE_H
#define NODE_H

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

#endif