#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <QVariant>

#include <memory>

namespace BinaryTree {

struct Node
{
    QVariant value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node() = default;
    explicit Node(QVariant nodeValue);
    Node(QVariant nodeValue, std::unique_ptr<Node> leftNode, std::unique_ptr<Node> rightNode);

    Node(const Node& other);
    Node& operator=(const Node& other);
    Node(Node&& other) noexcept = default;
    Node& operator=(Node&& other) noexcept = default;
    ~Node() = default;

    bool isLeaf() const;
    int size() const;
    int height() const;

private:
    static std::unique_ptr<Node> clone(const std::unique_ptr<Node>& node);
};

} // namespace BinaryTree

#endif // BINARY_TREE_HPP
