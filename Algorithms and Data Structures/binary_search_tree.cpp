#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

fstream in("input.txt", ios_base::in);
fstream out("output.txt", ios_base::out);

// node's class
class TNode {
public:
    explicit TNode(int key)
            : key_(key),
              height_(-1),
              length_(0)
              {
              }

    TNode *left_ = nullptr;
    TNode *right_ = nullptr;
    int key_;
    int height_;
    int length_; // length of the semipath with the root in that vertix
};

class BinaryTree {
public:
    BinaryTree()
            : root(nullptr),
              max_element(INT_MIN),
              root_max_path_key(INT_MIN)
              {
              }

    ~BinaryTree() {
        DestroyNode(root);
    }

    void Insert(int key) {
        if (abs(key) > max_element) {
            max_element = abs(key);
        }
        TNode **cur = &root;
        while (*cur) {
            TNode &node = **cur;
            if (key < node.key_) {
                cur = &node.left_;
            } else if (key > node.key_) {
                cur = &node.right_;
            } else {
                return;
            }
        }
        *cur = new TNode(key);
    }

    void Print() {
        PrintTree(root);
    }

    void Delete(int key) {
        root = DeleteKey(key, root);
    }

    /*
    void BackwardPass() {
        TNode *h = root;
        Backward(h); // i do it to write marks on each vertix - it's height
        max_element = INT_MIN;
        Inner(root); // do it to count the biggest semipath with the root in concrete vertix

        // finding the average-value vertix in our biggest semipath
        auto x = FindAverageVertix(FindVertix(root, root_max_path_key), max_element, root);
        if (x != nullptr) {
            root = DeleteKey(x->key_, root);
        }
    }
    */

private:

    static TNode* FindAverageVertix(TNode* node, int length, TNode* rt) {
        if (length % 2 == 1) {
            return nullptr;
        }
        if(node != nullptr) {
            vector<int> vec;
            vec.push_back(node->key_);
            if (node->left_ != nullptr && node->right_ != nullptr) {
                TNode *cur = node->left_;
                node = node->right_;
                vec.push_back(node->key_);
                vec.push_back(cur->key_);
                CountVector(node, vec);
                CountVector(cur, vec);
            } else if (node->left_ == nullptr && node->right_ != nullptr) {
                node = node->right_;
                vec.push_back(node->key_);
                CountVector(node, vec);
            } else if (node->left_ != nullptr && node->right_ == nullptr) {
                node = node->left_;
                vec.push_back(node->key_);
                CountVector(node, vec);
            }
            sort(vec.begin(), vec.end());
            return FindVertix(rt, vec[length / 2]);
        }
        return nullptr;
    }

    static void CountVector(TNode* node, vector<int>& vec) {
        while(node) {
            if (node->left_ != nullptr && node->right_ != nullptr) {
                if (node->left_->height_ >= node->right_->height_) {
                    node = node->left_;
                    vec.push_back(node->key_);
                } else {
                    node = node->right_;
                    vec.push_back(node->key_);
                }
            } else if (node->left_ == nullptr && node->right_ != nullptr) {
                node = node->right_;
                vec.push_back(node->key_);
            } else if (node->left_ != nullptr && node->right_ == nullptr) {
                node = node->left_;
                vec.push_back(node->key_);
            } else {
                break;
            }
        }
    }

    static TNode *FindVertix(TNode *node, int key) {
        if (node == nullptr) {
            return nullptr;
        }
        TNode **cur = &node;
        while (*cur) {
            TNode &ver = **cur;
            if (key < ver.key_) {
                cur = &ver.left_;
            } else if (key > ver.key_) {
                cur = &ver.right_;
            } else {
                return &ver;
            }
        }
        return nullptr;
    }

    void Inner(TNode *node) {
        if (node != nullptr) {
            if (node->left_ != nullptr) {
                Inner(node->left_);
            }
            if (node->length_ > max_element) {
                max_element = node->length_;
                root_max_path_key = node->key_;
            }
            if (node->right_ != nullptr) {
                Inner(node->right_);
            }
        }
    }

    static void Backward(TNode *node) {
        if (node->left_ != nullptr) {
            Backward(node->left_);
        }
        if (node->right_ != nullptr) {
            Backward(node->right_);
        }
        if (node->left_ == nullptr && node->right_ == nullptr) {
            node->height_ = 0;
            node->length_ = 0;
        } else if (node->left_ != nullptr && node->right_ != nullptr) {
            node->height_ = max(node->left_->height_, node->right_->height_) + 1;
            node->length_ = node->left_->height_ + node->right_->height_ + 2;
        } else if (node->left_ == nullptr) {
            node->height_ = node->right_->height_ + 1;
            node->length_ = node->height_;
        } else {
            node->height_ = node->left_->height_ + 1;
            node->length_ = node->height_;
        }

    }

    static TNode *FindMin(TNode *node) {
        if (node->left_ != nullptr) {
            return FindMin(node->left_);
        } else {
            return node;
        }
    }

    static TNode *DeleteKey(int key, TNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (key < node->key_) {
            node->left_ = DeleteKey(key, node->left_);
            return node;
        } else if (key > node->key_) {
            node->right_ = DeleteKey(key, node->right_);
            return node;
        }

        if (node->left_ == nullptr) {
            return node->right_;
        } else if (node->right_ == nullptr) {
            return node->left_;
        } else {
            auto minKey = FindMin(node->right_)->key_;
            node->key_ = minKey;
            node->right_ = DeleteKey(minKey, node->right_);
            return node;
        }
    }

    static void PrintTree(TNode *node) {
        if (node != nullptr) {
            out << node->key_ << "\n";
            if (node->left_ != nullptr) {
                PrintTree(node->left_);
            }
            if (node->right_ != nullptr) {
                PrintTree(node->right_);
            }
        }
    }

    static void DestroyNode(TNode *node) {
        if (node) {
            DestroyNode(node->left_);
            DestroyNode(node->right_);
            delete node;
        }
    }

    int max_element;
    int root_max_path_key;
    TNode *root;
};
