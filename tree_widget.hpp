#ifndef TREE_WIDGET_HPP
#define TREE_WIDGET_HPP

#include <QGraphicsView>

#include "binary_tree.hpp"
#include "binary_tree_visualizer.hpp"

#include <memory>

class QGraphicsScene;

class TreeWidget final : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget* parent = nullptr);

    void setTree(std::unique_ptr<BinaryTree::Node> root);

private:
    std::unique_ptr<BinaryTree::Node> m_root;
    BinaryTree::Visualizer m_visualizer;
    QGraphicsScene* m_scene;
};

#endif // TREE_WIDGET_HPP
