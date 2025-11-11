#include "GraphWidget.h"
#include <Nebula.h>
#include <QtGui/QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <RandomUtilities.h>
#include <StarSystem.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent) {
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        if (isDetailMode) {
            this->update();
        }
    });
}

void GraphWidget::setGraph(const std::vector<W_Vertex> &v, const std::vector<W_Edge> &e,
                           const std::vector<CelestialObject *> *objects) {
    vertices = v;
    edges = e;
    this->celestialObjectsPtr = objects;
    update();
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint clickPt = event->position().toPoint();
        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto &vertex = vertices[i];
            if (vertex.id < 0) continue;
            int dx = clickPt.x() - vertex.x;
            int dy = clickPt.y() - vertex.y;
            if (dx * dx + dy * dy <= 10 * 10) {
                emit vertexDoubleClicked(vertex.id);
                return;
            }
        }
    }
    QWidget::mouseDoubleClickEvent(event);
}

int GraphWidget::getDetailedVertexId() const {
    return detailedVertexId;
}

void GraphWidget::zoomToVertex(int vertexId) {
    this->detailedVertexId = vertexId;
    this->isDetailMode = true;
    this->update();
    animationTimer->start(50);
}

void GraphWidget::resetZoom() {
    this->detailedVertexId = -1;
    this->isDetailMode = false;
    this->update();
    animationTimer->stop();
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    if (vertices.empty()) return;

    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!celestialObjectsPtr) return;

    if (isDetailMode && detailedVertexId != -1 && celestialObjectsPtr) {
        if (detailedVertexId < 0 || detailedVertexId >= (int) vertices.size() ||
            detailedVertexId < 0 || detailedVertexId >= (int) celestialObjectsPtr->size()) {
            isDetailMode = false;
            detailedVertexId = -1;
            return;
        }

        CelestialObject *obj = (*celestialObjectsPtr)[detailedVertexId];
        if (!obj) {
            isDetailMode = false;
            detailedVertexId = -1;
            return;
        }

        const W_Vertex &focusedV = vertices[detailedVertexId];
        if (focusedV.id == -1) {
            isDetailMode = false;
            detailedVertexId = -1;
            return;
        }

        double scaleFactor = 3.0;
        painter.translate(width() / 2, height() / 2);
        painter.scale(scaleFactor, scaleFactor);
        painter.translate(-focusedV.x, -focusedV.y);

        int starRadius = 24;

        if (obj->getType() == "Nebula") {
            Nebula *nebula = dynamic_cast<Nebula *>(obj);
            if (nebula) {
                int nebulaRadius = 50;

                QRadialGradient gradient(focusedV.x, focusedV.y, nebulaRadius);

                Nebula::nebulaType type = nebula->getNebulaType();
                if (type == Nebula::nebulaType::Planetary) {
                    gradient.setColorAt(0, QColor(100, 255, 255, 200));
                    gradient.setColorAt(0.5, QColor(50, 150, 255, 150));
                    gradient.setColorAt(1, QColor(20, 50, 150, 50));
                    painter.setPen(QPen(Qt::cyan, 2));
                } else if (type == Nebula::nebulaType::Supernova) {
                    gradient.setColorAt(0, QColor(255, 255, 150, 220));
                    gradient.setColorAt(0.4, QColor(255, 100, 50, 180));
                    gradient.setColorAt(1, QColor(150, 0, 0, 40));
                    painter.setPen(QPen(Qt::red, 2));
                } else if (type == Nebula::nebulaType::Emission) {
                    gradient.setColorAt(0, QColor(255, 100, 150, 200));
                    gradient.setColorAt(0.5, QColor(200, 50, 100, 150));
                    gradient.setColorAt(1, QColor(100, 20, 50, 50));
                    painter.setPen(QPen(Qt::magenta, 2));
                } else if (type == Nebula::nebulaType::Reflection) {
                    gradient.setColorAt(0, QColor(150, 200, 255, 200));
                    gradient.setColorAt(0.5, QColor(80, 120, 200, 150));
                    gradient.setColorAt(1, QColor(30, 50, 120, 50));
                    painter.setPen(QPen(Qt::blue, 2));
                } else if (type == Nebula::nebulaType::Dark) {
                    gradient.setColorAt(0, QColor(80, 60, 100, 180));
                    gradient.setColorAt(0.5, QColor(50, 30, 70, 120));
                    gradient.setColorAt(1, QColor(20, 10, 30, 60));
                    painter.setPen(QPen(QColor(100, 80, 120), 2));
                } else {
                    gradient.setColorAt(0, QColor(200, 200, 255, 200));
                    gradient.setColorAt(0.5, QColor(100, 100, 200, 150));
                    gradient.setColorAt(1, QColor(50, 50, 100, 50));
                    painter.setPen(QPen(Qt::white, 2));
                }

                painter.setBrush(gradient);
                painter.drawEllipse(focusedV.x - nebulaRadius, focusedV.y - nebulaRadius,
                                    nebulaRadius * 2, nebulaRadius * 2);

                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(focusedV.x - nebulaRadius - 5, focusedV.y - nebulaRadius - 5,
                                    (nebulaRadius + 5) * 2, (nebulaRadius + 5) * 2);

                QFont font = painter.font();
                font.setPointSize(12);
                font.setBold(false);
                font.setFamily("Ravie");
                painter.setFont(font);
                painter.setPen(QPen(QColor(200, 200, 200), 2)); // Світло-сірий колір
                painter.drawText(focusedV.x - nebulaRadius - 150,
                                 focusedV.y - nebulaRadius + 155,
                                 focusedV.name);
            }
        } else if (obj->getType() == "StarSystem") {
            StarSystem *system = dynamic_cast<StarSystem *>(obj);
            if (system) {
                Star::starType type = system->getStar().getStarType();
                if (type == Star::starType::White_Dwarf) {
                    painter.setBrush(Qt::lightGray);
                    painter.setPen(Qt::darkMagenta);
                } else if (type == Star::starType::Brown_Dwarf) {
                    painter.setBrush(Qt::color1);
                    painter.setPen(Qt::white);
                } else if (type == Star::starType::Main_sequence_Star) {
                    painter.setBrush(Qt::yellow);
                    painter.setPen(Qt::color0);
                } else if (type == Star::starType::Neutron_Star) {
                    painter.setBrush(Qt::darkBlue);
                    painter.setPen(Qt::white);
                } else if (type == Star::starType::Red_Giant) {
                    painter.setBrush(Qt::red);
                    painter.setPen(Qt::darkRed);
                } else if (type == Star::starType::Red_Dwarf) {
                    painter.setBrush(Qt::green);
                    painter.setPen(Qt::red);
                } else {
                    painter.setBrush(Qt::white);
                    painter.setPen(Qt::gray);
                }

                painter.drawEllipse(focusedV.x - starRadius, focusedV.y - starRadius,
                                    starRadius * 2, starRadius * 2);

                painter.setPen(QPen(QColor(200, 200, 200), 2)); // Світло-сірий колір
                QFont font = painter.font();
                font.setPointSize(8);
                font.setBold(false);
                font.setFamily("Ravie");
                painter.setFont(font);
                painter.drawText(focusedV.x - starRadius - 170,
                                 focusedV.y + starRadius + 85,
                                 focusedV.name);

                double minMass = std::numeric_limits<double>::max();
                double maxMass = std::numeric_limits<double>::lowest();

                for (const auto &p: system->getPlanets()) {
                    minMass = std::min(minMass, p.getMass());
                    maxMass = std::max(maxMass, p.getMass());
                }

                if (maxMass == minMass) {
                    maxMass = minMass + 1.0;
                }
                int maxPlanetRadius = 5;
                for (const auto &p: system->getPlanets()) {
                    double massNorm = (p.getMass() - minMass) / (maxMass - minMass);
                    int planetRadius = int(4.0 + 8.0 * massNorm);
                    maxPlanetRadius = std::max(maxPlanetRadius, planetRadius);
                }

                int orbitStart = starRadius + maxPlanetRadius + 12;

                static RandomGenerator rng;
                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet &planet = system->getPlanets()[i];
                    if (planet.getColor() == QColor(0, 0, 0) || planet.getColor() == QColor()) {
                        planet.setColor(rng.getRandomColor());
                    }
                }

                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet &planet = system->getPlanets()[i];

                    double orbitRadiusBase = orbitStart + 40.0 * (i / double(std::max(
                                                                      (int) system->getPlanets().size() - 1, 1)));

                    double time = QDateTime::currentMSecsSinceEpoch() / 1000.0;
                    double angularSpeed = 1.0 / std::sqrt(planet.getOrbit() + 1.0);
                    double angle = time * angularSpeed * 0.5;
                    int planetX = focusedV.x + orbitRadiusBase * std::cos(angle);
                    int planetY = focusedV.y + orbitRadiusBase * std::sin(angle);

                    painter.setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(focusedV.x - (int) orbitRadiusBase,
                                        focusedV.y - (int) orbitRadiusBase,
                                        orbitRadiusBase * 2, orbitRadiusBase * 2);
                    painter.setPen(Qt::NoPen);

                    double massNorm = (planet.getMass() - minMass) / (maxMass - minMass);
                    double radius2 = 4.0 + 8.0 * massNorm;

                    painter.setBrush(planet.getColor());
                    painter.drawEllipse(planetX - radius2, planetY - radius2,
                                        radius2 * 2, radius2 * 2);
                }
            }
        }
    } else {
        painter.setPen(Qt::white);

        for (const auto &edge: edges) {
            if (edge.from < 0 || edge.from >= (int) vertices.size()) continue;
            if (edge.to < 0 || edge.to >= (int) vertices.size()) continue;
            const W_Vertex &fromV = vertices[edge.from];
            const W_Vertex &toV = vertices[edge.to];
            if (fromV.id == -1 || toV.id == -1) continue;

            painter.drawLine(fromV.x, fromV.y, toV.x, toV.y);
            int mx = (fromV.x + toV.x) / 2;
            int my = (fromV.y + toV.y) / 2;
            painter.drawText(mx, my, QString::number(edge.weight));
        }

        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto &vertex = vertices[i];
            if (vertex.id == -1) continue;

            int vertexRadius = 12;

            if (i < celestialObjectsPtr->size() && (*celestialObjectsPtr)[i]) {
                CelestialObject *obj = (*celestialObjectsPtr)[i];

                if (obj->getType() == "StarSystem") {
                    StarSystem *system = dynamic_cast<StarSystem *>(obj);
                    if (system) {
                        Star::starType type = system->getStar().getStarType();
                        if (type == Star::starType::White_Dwarf) {
                            painter.setBrush(Qt::lightGray);
                            painter.setPen(QPen(Qt::darkMagenta, 2));
                        } else if (type == Star::starType::Brown_Dwarf) {
                            painter.setBrush(Qt::color1);
                            painter.setPen(QPen(Qt::white, 2));
                        } else if (type == Star::starType::Main_sequence_Star) {
                            painter.setBrush(Qt::yellow);
                            painter.setPen(QPen(Qt::color0, 2));
                        } else if (type == Star::starType::Neutron_Star) {
                            painter.setBrush(Qt::darkBlue);
                            painter.setPen(QPen(Qt::white, 2));
                        } else if (type == Star::starType::Red_Giant) {
                            painter.setBrush(Qt::red);
                            painter.setPen(QPen(Qt::darkRed, 2));
                        } else if (type == Star::starType::Red_Dwarf) {
                            painter.setBrush(Qt::green);
                            painter.setPen(QPen(Qt::red, 2));
                        } else {
                            painter.setBrush(Qt::white);
                            painter.setPen(QPen(Qt::gray, 2));
                        }
                    }
                } else if (obj->getType() == "Nebula") {
                    Nebula *nebula = dynamic_cast<Nebula *>(obj);
                    if (nebula) {
                        QRadialGradient gradient(vertex.x, vertex.y, vertexRadius);

                        Nebula::nebulaType type = nebula->getNebulaType();
                        if (type == Nebula::nebulaType::Planetary) {
                            gradient.setColorAt(0, QColor(100, 255, 255, 200));
                            gradient.setColorAt(0.5, QColor(50, 150, 255, 150));
                            gradient.setColorAt(1, QColor(20, 50, 150, 100));
                            painter.setPen(QPen(Qt::cyan, 2));
                        } else if (type == Nebula::nebulaType::Supernova) {
                            gradient.setColorAt(0, QColor(255, 255, 150, 220));
                            gradient.setColorAt(0.4, QColor(255, 100, 50, 180));
                            gradient.setColorAt(1, QColor(150, 0, 0, 100));
                            painter.setPen(QPen(Qt::red, 2));
                        } else if (type == Nebula::nebulaType::Emission) {
                            gradient.setColorAt(0, QColor(255, 100, 150, 200));
                            gradient.setColorAt(0.5, QColor(200, 50, 100, 150));
                            gradient.setColorAt(1, QColor(100, 20, 50, 100));
                            painter.setPen(QPen(Qt::magenta, 2));
                        } else if (type == Nebula::nebulaType::Reflection) {
                            gradient.setColorAt(0, QColor(150, 200, 255, 200));
                            gradient.setColorAt(0.5, QColor(80, 120, 200, 150));
                            gradient.setColorAt(1, QColor(30, 50, 120, 100));
                            painter.setPen(QPen(Qt::blue, 2));
                        } else if (type == Nebula::nebulaType::Dark) {
                            gradient.setColorAt(0, QColor(80, 60, 100, 180));
                            gradient.setColorAt(0.5, QColor(50, 30, 70, 120));
                            gradient.setColorAt(1, QColor(20, 10, 30, 80));
                            painter.setPen(QPen(QColor(100, 80, 120), 2));
                        } else {
                            gradient.setColorAt(0, QColor(200, 200, 255, 200));
                            gradient.setColorAt(0.5, QColor(100, 100, 200, 150));
                            gradient.setColorAt(1, QColor(50, 50, 100, 100));
                            painter.setPen(QPen(Qt::white, 2));
                        }

                        painter.setBrush(gradient);
                    }
                } else {
                    painter.setBrush(Qt::cyan);
                    painter.setPen(QPen(Qt::black, 2));
                }
            } else {
                painter.setBrush(Qt::cyan);
                painter.setPen(QPen(Qt::black, 2));
            }

            painter.drawEllipse(vertex.x - vertexRadius, vertex.y - vertexRadius,
                                vertexRadius * 2, vertexRadius * 2);

            painter.setPen(Qt::black);
            QFont idFont = painter.font();
            idFont.setPointSize(8);
            idFont.setBold(true);
            painter.setFont(idFont);
            painter.drawText(vertex.x - 6, vertex.y + 4, QString::number(vertex.id));

            painter.setPen(Qt::white);
            QFont nameFont = painter.font();
            nameFont.setPointSize(9);
            painter.setFont(nameFont);
            painter.drawText(vertex.x + vertexRadius + 5, vertex.y + 4, vertex.name);
        }
    }
}
