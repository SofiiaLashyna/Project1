
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H
#include <QtWidgets/QWidget>
#include <vector>

struct W_Edge {
    int from;
    int to;
    int weight;
};

struct W_Vertex {
    int id;
    int x, y; // координати на вікні
    QString name;
};

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QWidget *parent = nullptr);

    void setGraph(const std::vector<W_Vertex>& v, const std::vector<W_Edge>& e);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<W_Vertex> vertices;
    std::vector<W_Edge> edges;
};

#endif //GRAPHWIDGET_H