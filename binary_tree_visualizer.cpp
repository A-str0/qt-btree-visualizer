#include "binary_tree_visualizer.hpp"

#include <QFontMetrics>
#include <QMetaType>
#include <QPainter>
#include <QPen>

#include <algorithm>
#include <limits>

namespace BinaryTree {

void Visualizer::draw(QPainter& painter, const Node* root, const QRect& bounds) const
{
    if (!root)
        return;

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRect drawingBounds = bounds.adjusted(kMargin, kMargin, -kMargin, -kMargin);
    drawSubtree(
        painter,
        root,
        0,
        drawingBounds.left(),
        drawingBounds.right(),
        drawingBounds.top());

    painter.restore();
}

QSize Visualizer::sizeHint(const Node* root) const
{
    if (!root)
        return QSize((kMargin * 2) + kNodeWidth, (kMargin * 2) + kNodeHeight);

    const int height = root->height();
    const long long leafCount = leafSlotCount(height);
    const long long totalWidth = (kMargin * 2LL)
        + (leafCount * kNodeWidth)
        + ((leafCount - 1) * kHorizontalSpacing);
    const long long totalHeight = (kMargin * 2LL)
        + kNodeHeight
        + (static_cast<long long>(height - 1) * kLevelHeight);

    return QSize(
        static_cast<int>(std::min(totalWidth, static_cast<long long>(std::numeric_limits<int>::max()))),
        static_cast<int>(std::min(totalHeight, static_cast<long long>(std::numeric_limits<int>::max()))));
}

void Visualizer::drawSubtree(
    QPainter& painter,
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
    const QPoint center(centerX, centerY);
    const int middle = left + ((right - left) / 2);
    const int childCenterY = top + ((depth + 1) * kLevelHeight) + (kNodeHeight / 2);

    if (node->left) {
        const int childLeft = left;
        const int childRight = middle;
        const int childCenterX = childLeft + ((childRight - childLeft) / 2);
        const QPoint childCenter(childCenterX, childCenterY);
        painter.setPen(QPen(QColor(QStringLiteral("#9CA3AF")), 1));
        painter.drawLine(
            QPoint(center.x(), center.y() + (kNodeHeight / 2)),
            QPoint(childCenter.x(), childCenter.y() - (kNodeHeight / 2)));
        drawSubtree(painter, node->left.get(), depth + 1, childLeft, childRight, top);
    }

    if (node->right) {
        const int childLeft = middle;
        const int childRight = right;
        const int childCenterX = childLeft + ((childRight - childLeft) / 2);
        const QPoint childCenter(childCenterX, childCenterY);
        painter.setPen(QPen(QColor(QStringLiteral("#9CA3AF")), 1));
        painter.drawLine(
            QPoint(center.x(), center.y() + (kNodeHeight / 2)),
            QPoint(childCenter.x(), childCenter.y() - (kNodeHeight / 2)));
        drawSubtree(painter, node->right.get(), depth + 1, childLeft, childRight, top);
    }

    drawNode(painter, node, center);
}

void Visualizer::drawNode(QPainter& painter, const Node* node, const QPoint& center) const
{
    const QRect nodeRect(
        center.x() - (kNodeWidth / 2),
        center.y() - (kNodeHeight / 2),
        kNodeWidth,
        kNodeHeight);

    painter.setPen(QPen(QColor(QStringLiteral("#111827")), 1));
    painter.setBrush(QColor(QStringLiteral("#FFFFFF")));
    painter.drawRect(nodeRect);

    const QFontMetrics metrics(painter.font());
    const QString label = metrics.elidedText(nodeLabel(*node), Qt::ElideMiddle, kNodeWidth - 12);
    painter.setPen(QColor(QStringLiteral("#111827")));
    painter.drawText(nodeRect, Qt::AlignCenter, label);
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
