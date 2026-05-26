#include "tree_node_item.hpp"

#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

TreeNodeItem::TreeNodeItem(QString label, const QPointF& center)
    : m_label(std::move(label))
{
    setPos(center);
}

QRectF TreeNodeItem::boundingRect() const
{
    constexpr qreal adjust = 1.0;
    return QRectF(
        -kWidth / 2.0 - adjust,
        -kHeight / 2.0 - adjust,
        kWidth + 2.0 * adjust,
        kHeight + 2.0 * adjust);
}

void TreeNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const QRectF nodeRect(-kWidth / 2.0, -kHeight / 2.0, kWidth, kHeight);

    painter->setPen(QPen(QColor(QStringLiteral("#111827")), 1));
    painter->setBrush(QColor(QStringLiteral("#FFFFFF")));
    painter->drawRect(nodeRect);

    QFontMetrics metrics(painter->font());
    const QString elided = metrics.elidedText(m_label, Qt::ElideMiddle, kWidth - 12);
    painter->setPen(QColor(QStringLiteral("#111827")));
    painter->drawText(nodeRect, Qt::AlignCenter, elided);
}
