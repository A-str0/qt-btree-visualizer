#include "binary_tree.hpp"

#include <algorithm>
#include <utility>

namespace BinaryTree {

Node::Node(QVariant nodeValue)
    : value(std::move(nodeValue))
{
}

Node::Node(QVariant nodeValue, std::unique_ptr<Node> leftNode, std::unique_ptr<Node> rightNode)
    : value(std::move(nodeValue))
    , left(std::move(leftNode))
    , right(std::move(rightNode))
{
}

Node::Node(const Node& other)
    : value(other.value)
    , left(clone(other.left))
    , right(clone(other.right))
{
}

Node& Node::operator=(const Node& other)
{
    if (this == &other)
        return *this;

    value = other.value;
    left = clone(other.left);
    right = clone(other.right);
    return *this;
}

bool Node::isLeaf() const
{
    return !left && !right;
}

int Node::size() const
{
    const int leftSize = left ? left->size() : 0;
    const int rightSize = right ? right->size() : 0;
    return 1 + leftSize + rightSize;
}

int Node::height() const
{
    const int leftHeight = left ? left->height() : 0;
    const int rightHeight = right ? right->height() : 0;
    return 1 + std::max(leftHeight, rightHeight);
}

std::unique_ptr<Node> Node::clone(const std::unique_ptr<Node>& node)
{
    return node ? std::make_unique<Node>(*node) : nullptr;
}

} // namespace BinaryTree
