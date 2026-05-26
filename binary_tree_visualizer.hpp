#ifndef BINARY_TREE_VISUALIZER_HPP
#define BINARY_TREE_VISUALIZER_HPP

#include <QSizeF>

#include "binary_tree.hpp"

class QGraphicsScene;

namespace BinaryTree {

class Visualizer
{
public:
    void buildScene(QGraphicsScene* scene, const Node* root) const;
    QSizeF sizeHint(const Node* root) const;

private:
    static constexpr int kNodeWidth = 60;
    static constexpr int kNodeHeight = 32;
    static constexpr int kHorizontalSpacing = 24;
    static constexpr int kLevelHeight = 72;
    static constexpr int kMargin = 24;

    void addSubtree(QGraphicsScene* scene, const Node* node, int depth, int left, int right, int top) const;
    int leafSlotCount(int height) const;
    QString nodeLabel(const Node& node) const;
};

} // namespace BinaryTree

#endif // BINARY_TREE_VISUALIZER_HPP
