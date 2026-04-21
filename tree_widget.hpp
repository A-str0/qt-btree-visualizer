#ifndef TREE_WIDGET_HPP
#define TREE_WIDGET_HPP

#include <QWidget>

#include "binary_tree.hpp"
#include "binary_tree_visualizer.hpp"

#include <memory>

class QPaintEvent;

class TreeWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget* parent = nullptr);

    void setTree(std::unique_ptr<BinaryTree::Node> root);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::unique_ptr<BinaryTree::Node> m_root;
    BinaryTree::Visualizer m_visualizer;
};

#endif // TREE_WIDGET_HPP
