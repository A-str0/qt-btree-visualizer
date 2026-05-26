#include "tree_widget.hpp"

#include <QGraphicsScene>

TreeWidget::TreeWidget(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing, true);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void TreeWidget::setTree(std::unique_ptr<BinaryTree::Node> root)
{
    m_root = std::move(root);
    m_scene->clear();

    if (m_root) {
        m_visualizer.buildScene(m_scene, m_root.get());
        m_scene->setSceneRect(m_scene->itemsBoundingRect().adjusted(-24, -24, 24, 24));
    }
}
