#include "binary_tree_visualizer.hpp"

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QMetaType>
#include <QPen>

#include "tree_node_item.hpp"

#include <algorithm>
#include <limits>

namespace BinaryTree {

void Visualizer::buildScene(QGraphicsScene* scene, const Node* root) const
{
    if (!root)
        return;

    const QSizeF totalSize = sizeHint(root);
    const QRectF drawingBounds(
        kMargin, kMargin,
        totalSize.width() - 2.0 * kMargin,
        totalSize.height() - 2.0 * kMargin);

    addSubtree(
        scene,
        root,
        0,
        static_cast<int>(drawingBounds.left()),
        static_cast<int>(drawingBounds.right()),
        static_cast<int>(drawingBounds.top()));
}

QSizeF Visualizer::sizeHint(const Node* root) const
{
    if (!root)
        return QSizeF((kMargin * 2) + kNodeWidth, (kMargin * 2) + kNodeHeight);

    const int height = root->height();
    const long long leafCount = leafSlotCount(height);
    const long long totalWidth = (kMargin * 2LL)
        + (leafCount * kNodeWidth)
        + ((leafCount - 1) * kHorizontalSpacing);
    const long long totalHeight = (kMargin * 2LL)
        + kNodeHeight
        + (static_cast<long long>(height - 1) * kLevelHeight);

    return QSizeF(
        static_cast<qreal>(std::min(totalWidth, static_cast<long long>(std::numeric_limits<int>::max()))),
        static_cast<qreal>(std::min(totalHeight, static_cast<long long>(std::numeric_limits<int>::max()))));
}

void Visualizer::addSubtree(
    QGraphicsScene* scene,
    const Node* node,
    int depth,
    int left,
    int right,
    int top) const
{
    if (!node)
        return;

    const int centerX = left + ((right - left) / 2);
    const int centerY = top + (depth * kLevelHeight) + (kNodeHeight / 2);
    const QPointF center(centerX, centerY);
    const int middle = left + ((right - left) / 2);
    const int childCenterY = top + ((depth + 1) * kLevelHeight) + (kNodeHeight / 2);
    const QColor edgeColor(QStringLiteral("#9CA3AF"));
    const QPen edgePen(edgeColor, 1);

    if (node->left) {
        const int childLeft = left;
        const int childRight = middle;
        const int childCenterX = childLeft + ((childRight - childLeft) / 2);

        auto* line = scene->addLine(
            center.x(), center.y() + (kNodeHeight / 2),
            childCenterX, childCenterY - (kNodeHeight / 2),
            edgePen);
        line->setZValue(-1);

        addSubtree(scene, node->left.get(), depth + 1, childLeft, childRight, top);
    }

    if (node->right) {
        const int childLeft = middle;
        const int childRight = right;
        const int childCenterX = childLeft + ((childRight - childLeft) / 2);

        auto* line = scene->addLine(
            center.x(), center.y() + (kNodeHeight / 2),
            childCenterX, childCenterY - (kNodeHeight / 2),
            edgePen);
        line->setZValue(-1);

        addSubtree(scene, node->right.get(), depth + 1, childLeft, childRight, top);
    }

    auto* item = new TreeNodeItem(nodeLabel(*node), center);
    item->setZValue(0);
    scene->addItem(item);
}

int Visualizer::leafSlotCount(int height) const
{
    int leafCount = 1;

    for (int level = 1; level < height; ++level)
        leafCount *= 2;

    return leafCount;
}

QString Visualizer::nodeLabel(const Node& node) const
{
    if (!node.value.isValid() || node.value.isNull())
        return QStringLiteral("null");

    if (node.value.metaType().id() == QMetaType::Bool)
        return node.value.toBool() ? QStringLiteral("true") : QStringLiteral("false");

    const QString asString = node.value.toString();
    if (!asString.isEmpty())
        return asString;

    return QStringLiteral("<value>");
}

} // namespace BinaryTree
