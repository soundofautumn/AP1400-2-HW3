#ifndef BST_H
#define BST_H

#include <functional>
#include <iostream>


class BST {
public:
    class Node {
    public:
        Node(int value, Node *left, Node *right);

        Node();

        Node(const Node &node);

        int value;
        Node *left;
        Node *right;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);

        bool operator==(int other) const;

        std::strong_ordering operator<=>(int other) const;
    };

    BST();

    BST(const BST &bst);

    BST(BST &&bst) noexcept;

    BST(std::initializer_list<int> list);

    ~BST();

    Node *&get_root();

    void bfs(const std::function<void(Node *&node)> &func) const;

    [[nodiscard]] size_t length() const;

    bool add_node(int value);

    Node **find_node(int value);

    Node **find_parent(int value);

    Node **find_successor(int value);

    bool delete_node(int value);

    // ++bst
    BST &operator++();

    // bst++
    BST operator++(int);

    // =
    BST &operator=(const BST &bst);

    BST &operator=(BST &&bst) noexcept;

    friend std::ostream &operator<<(std::ostream &os, const BST &bst);

private:
    Node *root;
};

#endif //BST_H