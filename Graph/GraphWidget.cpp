#include "GraphWidget.h"
#include <QtGui/QPainter>
#include <QMouseEvent>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent) {}

void GraphWidget::setGraph(const std::vector<W_Vertex> &v, const std::vector<W_Edge> &e) {
    vertices = v;
    edges = e;
    update();
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::white);
    for (const auto& edge : edges) {
        const W_Vertex& fromV = vertices[edge.from];
        const W_Vertex& toV = vertices[edge.to];
        painter.drawLine(fromV.x, fromV.y, toV.x, toV.y);

        int mx = (fromV.x + toV.x) / 2;
        int my = (fromV.y + toV.y) / 2;
        painter.drawText(mx, my, QString::number(edge.weight));
    }

    painter.setBrush(Qt::cyan);
    for (const auto& vertex : vertices) {
        painter.setBrush(Qt::cyan);
        painter.setPen(Qt::black);
        painter.drawEllipse(vertex.x - 10, vertex.y - 10, 20, 20);
        painter.drawText(vertex.x - 5, vertex.y + 5, QString::number(vertex.id));
        painter.setPen(Qt::blue);
        painter.drawText(vertex.x + 12, vertex.y, vertex.name);
    }
}


