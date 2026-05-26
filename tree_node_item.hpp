#ifndef TREE_NODE_ITEM_HPP
#define TREE_NODE_ITEM_HPP

#include <QGraphicsItem>
#include <QString>

class TreeNodeItem final : public QGraphicsItem
{
public:
    TreeNodeItem(QString label, const QPointF& center);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QString m_label;

    static constexpr int kWidth = 60;
    static constexpr int kHeight = 32;
};

#endif // TREE_NODE_ITEM_HPP
