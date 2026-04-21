#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <QVariant>

namespace BinaryTree {
struct Node
{
    std::optional<QScopedPointer<Node>> leftChild;
    std::optional<QScopedPointer<Node>> rightSon;
};
} // namespace BinaryTree

#endif // BINARY_TREE_HPP
