#ifndef TREE_WIDGET_HPP
#define TREE_WIDGET_HPP

#include <QPainter>
#include <QWidget>

#include "binary_tree.hpp"

class TreeWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QScopedPointer<QPainter> painter, QWidget* parent = nullptr);

    void setTree(QScopedPointer<BinaryTree::Node> root);

private:
    QScopedPointer<BinaryTree::Node> m_root;
    QScopedPointer<QPainter> m_painter;

    void drawNode(QScopedPointer<BinaryTree::Node> node, int x, int y) const;
};

#endif // TREE_WIDGET_HPP
