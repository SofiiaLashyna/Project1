
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H
#include <CelestialObject.h>
#include <QtWidgets/QWidget>
#include <vector>
#include <QTimer>

struct W_Edge {
    int from;
    int to;
    int weight;
};

struct W_Vertex {
    int id;
    double x, y;
    QString name;
};

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QWidget *parent = nullptr);

    void setGraph(const std::vector<W_Vertex>& v, const std::vector<W_Edge>& e, const std::vector<CelestialObject*>* objects);
    void zoomToVertex(int vertexId);
    void resetZoom();
    void setSelectedNodes(int id1, int id2);
    int getDetailedVertexId() const;
    signals:
        void vertexDoubleClicked(int vertexId);
        void vertexClicked(int id);
        void backgroundClicked();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    std::vector<W_Vertex> vertices;
    std::vector<W_Edge> edges;
    const std::vector<CelestialObject*>* celestialObjectsPtr = nullptr;
    bool isDetailMode = false;
    int detailedVertexId = -1;
    QTimer *animationTimer = nullptr;
    int selectedId1 = -1;
    int selectedId2 = -1;
};

#endif //GRAPHWIDGET_H