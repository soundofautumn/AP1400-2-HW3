#include "bst.h"
#include <queue>
#include <iostream>

BST::Node::Node(int value, Node *left, Node *right) : value(value), left(left), right(right) {
}

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {
}

BST::Node::Node(const BST::Node &node) = default;

std::ostream &operator<<(std::ostream &os, const BST::Node &node) {
    os << node.value;
    return os;
}

bool BST::Node::operator==(int other) const {
    return this->value == other;
}

std::strong_ordering BST::Node::operator<=>(int other) const {
    return this->value <=> other;
}

BST::BST() : root(nullptr) {
}

BST::BST(const BST &bst) : root(nullptr) {
    bst.bfs([this](Node *&node) { add_node(node->value); });
}

BST::BST(BST &&bst) noexcept: root(bst.root) {
    bst.root = nullptr;
}

BST::BST(std::initializer_list<int> list) : root(nullptr) {
    for (auto &i: list)
        add_node(i);
}

BST::~BST() {
    std::vector<Node *> nodes;
    bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
    for (auto &node: nodes)
        delete node;
}

BST::Node *&BST::get_root() {
    return root;
}

void BST::bfs(const std::function<void(BST::Node *&node)> &func) const {
    if (root == nullptr) {
        return;
    }
    std::queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        func(node);
        if (node->left)
            q.push(node->left);
        if (node->right)
            q.push(node->right);
    }
}

size_t BST::length() const {
    size_t len{0};
    bfs([&len](Node *&node) { len++; });
    return len;
}

bool BST::add_node(int value) {
    if (!root) {
        root = new Node(value, nullptr, nullptr);
        return true;
    }
    Node **node = &root;
    while (*node) {
        if (**node == value)
            return false;
        if (value < **node)
            node = &(*node)->left;
        else
            node = &(*node)->right;
    }
    *node = new Node(value, nullptr, nullptr);
    return true;
}

BST::Node **BST::find_node(int value) {
    Node **node = &root;
    while (*node) {
        if (**node == value)
            return node;
        if (value < **node)
            node = &(*node)->left;
        else
            node = &(*node)->right;
    }
    return nullptr;
}

BST::Node **BST::find_parent(int value) {
    Node **node = &root;
    while (*node) {
        if (((*node)->left && *(*node)->left == value) ||
            ((*node)->right && *(*node)->right == value)) {
            return node;
        }
        if (value < *(*node))
            node = &(*node)->left;
        else
            node = &(*node)->right;
    }
    return nullptr;
}

BST::Node **BST::find_successor(int value) {
    Node **node = find_node(value);
    if (!node)
        return nullptr;
    if ((*node)->left) {
        node = &(*node)->left;
        while ((*node)->right)
            node = &(*node)->right;
        return node;
    }
    return nullptr;
}

bool BST::delete_node(int value) {
    Node **node = find_node(value);
    if (!node)
        return false;
    if ((*node)->left && (*node)->right) {
        Node **successor = find_successor(value);
        (*node)->value = (*successor)->value;
        node = successor;
    }
    Node *tmp = *node;
    *node = (*node)->left ? (*node)->left : (*node)->right;
    delete tmp;
    return true;
}

BST &BST::operator++() {
    bfs([](Node *&node) { node->value++; });
    return *this;
}

BST BST::operator++(int) {
    BST tmp(*this);
    ++(*this);
    return tmp;
}

BST &BST::operator=(const BST &bst) {
    if (this == &bst)
        return *this;
    std::vector<Node *> nodes;
    bfs([&nodes](Node *&node) { nodes.push_back(node); });
    for (auto &node: nodes)
        delete node;
    root = nullptr;
    bst.bfs([this](Node *&node) { add_node(node->value); });
    return *this;
}

BST &BST::operator=(BST &&bst) noexcept {
    if (this == &bst)
        return *this;
    std::vector<Node *> nodes;
    bfs([&nodes](Node *&node) { nodes.push_back(node); });
    for (auto &node: nodes)
        delete node;
    root = nullptr;
    root = bst.root;
    bst.root = nullptr;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const BST &bst) {
    bst.bfs([&os](BST::Node *&node) { os << *node << " "; });
    return os;
}
