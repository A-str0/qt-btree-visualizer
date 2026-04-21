#ifndef BINARY_TREE_VISUALIZER_HPP
#define BINARY_TREE_VISUALIZER_HPP

#include <QPoint>
#include <QRect>
#include <QSize>
#include <QString>

#include "binary_tree.hpp"

class QPainter;

namespace BinaryTree {

class Visualizer
{
public:
    void draw(QPainter& painter, const Node* root, const QRect& bounds) const;
    QSize sizeHint(const Node* root) const;

private:
    static constexpr int kNodeWidth = 60;
    static constexpr int kNodeHeight = 32;
    static constexpr int kHorizontalSpacing = 24;
    static constexpr int kLevelHeight = 72;
    static constexpr int kMargin = 24;

    void drawSubtree(QPainter& painter, const Node* node, int depth, int left, int right, int top) const;
    void drawNode(QPainter& painter, const Node* node, const QPoint& center) const;
    int leafSlotCount(int height) const;
    QString nodeLabel(const Node& node) const;
};

} // namespace BinaryTree

#endif // BINARY_TREE_VISUALIZER_HPP
