#include "tree_widget.hpp"

#include <QPaintEvent>
#include <QPainter>
#include <QPalette>

TreeWidget::TreeWidget(QWidget* parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
}

void TreeWidget::setTree(std::unique_ptr<BinaryTree::Node> root)
{
    m_root = std::move(root);
    updateGeometry();
    update();
}

QSize TreeWidget::minimumSizeHint() const { return m_visualizer.sizeHint(m_root.get()); }

QSize TreeWidget::sizeHint() const { return minimumSizeHint(); }

void TreeWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), palette().brush(QPalette::Base));

    if (!m_root) {
        painter.setPen(palette().color(QPalette::Mid));
        painter.drawText(
            rect().adjusted(16, 16, -16, -16),
            Qt::AlignCenter,
            tr("Load a JSON file to display the tree."));
        return;
    }

    m_visualizer.draw(painter, m_root.get(), rect());
}
