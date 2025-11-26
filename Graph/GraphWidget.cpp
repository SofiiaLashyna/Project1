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

void GraphWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint clickPt = event->position().toPoint();
        bool clickedOnVertex = false;
        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto &vertex = vertices[i];
            if (vertex.id < 0) continue;
            int dx = clickPt.x() - vertex.x;
            int dy = clickPt.y() - vertex.y;
            if (dx * dx + dy * dy <= 10 * 10) {
                emit vertexClicked(vertex.id);
                clickedOnVertex = true;
                return;
            }
        }
        if (!clickedOnVertex) {
            emit backgroundClicked();
        }
    }
    QWidget::mousePressEvent(event);
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

void GraphWidget::setHighlightedNodes(const std::vector<int>& ids) {
    highlightedIds.clear();
    for (int id : ids) {
        highlightedIds.insert(id);
    }
    update();
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
        painter.translate(width() / 2.0, height() / 2.0);
        painter.scale(scaleFactor, scaleFactor);
        painter.translate(-focusedV.x, -focusedV.y);

        double starRadius = 24.0;
        QPointF center(focusedV.x, focusedV.y);

        if (obj->getType() == "Nebula") {
            Nebula *nebula = dynamic_cast<Nebula *>(obj);
            if (nebula) {
                double nebulaRadius = 50.0;

                QRadialGradient gradient(center, nebulaRadius);
                Nebula::nebulaType type = nebula->getNebulaType();

                painter.setPen(Qt::NoPen);
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
                painter.drawEllipse(center, nebulaRadius, nebulaRadius);

                RandomGenerator rng(focusedV.id * 9999);

                int starCount = static_cast<int>(nebulaRadius * 2.5);

                for (int s = 0; s < starCount; ++s) {
                    double angle = rng.getDouble(0, 2 * M_PI);

                    double r = std::sqrt(rng.getDouble(0.0, 1.0)) * (nebulaRadius * 0.9);

                    double sx = center.x() + r * std::cos(angle);
                    double sy = center.y() + r * std::sin(angle);

                    if (s % 7 == 0) {
                        painter.setPen(QPen(QColor(255, 255, 255, 220), 1.5)); // Яскраві
                    } else {
                        painter.setPen(QPen(QColor(255, 255, 255, 140), 1.0)); // Тьмяні
                    }
                    painter.drawPoint(QPointF(sx, sy));
                }

                painter.setPen(Qt::NoPen);

                QFont font = painter.font();
                font.setPointSize(12);
                font.setBold(false);
                font.setFamily("Ravie");
                painter.setFont(font);
                painter.setPen(QPen(QColor(200, 200, 200), 2));
                painter.drawText(QPointF(focusedV.x - nebulaRadius - 150,
                                         focusedV.y - nebulaRadius + 155),
                                 focusedV.name);
            }
        } else if (obj->getType() == "StarSystem") {
            StarSystem *system = dynamic_cast<StarSystem *>(obj);
            if (system) {
                Star::starType type = system->getStar().getStarType();
                if (type == Star::starType::White_Dwarf) {
                    painter.setBrush(Qt::lightGray); painter.setPen(Qt::darkMagenta);
                } else if (type == Star::starType::Brown_Dwarf) {
                    painter.setBrush(Qt::color1); painter.setPen(Qt::white);
                } else if (type == Star::starType::Main_sequence_Star) {
                    painter.setBrush(Qt::yellow); painter.setPen(Qt::color0);
                } else if (type == Star::starType::Neutron_Star) {
                    painter.setBrush(Qt::darkBlue); painter.setPen(Qt::white);
                } else if (type == Star::starType::Red_Giant) {
                    painter.setBrush(Qt::red); painter.setPen(Qt::darkRed);
                } else if (type == Star::starType::Red_Dwarf) {
                    painter.setBrush(Qt::green); painter.setPen(Qt::red);
                } else {
                    painter.setBrush(Qt::white); painter.setPen(Qt::gray);
                }

                painter.drawEllipse(center, starRadius, starRadius);

                painter.setPen(QPen(QColor(200, 200, 200), 2));
                QFont font = painter.font();
                font.setPointSize(8);
                font.setBold(false);
                font.setFamily("Ravie");
                painter.setFont(font);
                painter.drawText(QPointF(focusedV.x - starRadius - 170,
                                         focusedV.y + starRadius + 85),
                                 focusedV.name);

                double minMass = std::numeric_limits<double>::max();
                double maxMass = std::numeric_limits<double>::lowest();

                for (const auto &p: system->getPlanets()) {
                    minMass = std::min(minMass, p.getMass());
                    maxMass = std::max(maxMass, p.getMass());
                }
                if (maxMass == minMass) maxMass = minMass + 1.0;

                int maxPlanetRadius = 5;
                for (const auto &p: system->getPlanets()) {
                    double massNorm = (p.getMass() - minMass) / (maxMass - minMass);
                    int planetRadius = int(4.0 + 8.0 * massNorm);
                    maxPlanetRadius = std::max(maxPlanetRadius, planetRadius);
                }

                double orbitStart = starRadius + maxPlanetRadius + 12.0;

                static RandomGenerator rng;
                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet &planet = system->getPlanets()[i];
                    if (planet.getColor() == QColor(0, 0, 0) || planet.getColor() == QColor()) {
                        planet.setColor(rng.getRandomColor());
                    }
                }

                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet &planet = system->getPlanets()[i];

                    double orbitRadiusBase = orbitStart + 40.0 * (i / double(std::max((int)system->getPlanets().size() - 1, 1)));

                    double time = QDateTime::currentMSecsSinceEpoch() / 1000.0;
                    double angularSpeed = 1.0 / std::sqrt(planet.getOrbit() + 1.0);
                    double angle = time * angularSpeed * 0.5;

                    double planetX = focusedV.x + orbitRadiusBase * std::cos(angle);
                    double planetY = focusedV.y + orbitRadiusBase * std::sin(angle);
                    QPointF planetCenter(planetX, planetY);

                    painter.setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(center, orbitRadiusBase, orbitRadiusBase);
                    painter.setPen(Qt::NoPen);

                    double massNorm = (planet.getMass() - minMass) / (maxMass - minMass);
                    double radius2 = 4.0 + 8.0 * massNorm;

                    painter.setBrush(planet.getColor());
                    painter.drawEllipse(planetCenter, radius2, radius2);
                }
            }
        }
    }
    else {
        painter.setPen(QPen(Qt::white, 1, Qt::DashLine));

        for (const auto &edge: edges) {
            if (edge.from < 0 || edge.from >= (int) vertices.size()) continue;
            if (edge.to < 0 || edge.to >= (int) vertices.size()) continue;
            const W_Vertex &fromV = vertices[edge.from];
            const W_Vertex &toV = vertices[edge.to];
            if (fromV.id == -1 || toV.id == -1) continue;

            QPointF p1(fromV.x, fromV.y);
            QPointF p2(toV.x, toV.y);
            painter.drawLine(p1, p2);

            QPointF midPoint = (p1 + p2) / 2.0;

            painter.setPen(Qt::yellow);
            painter.drawText(midPoint, QString::number(edge.weight));
            painter.setPen(QPen(Qt::white, 1, Qt::DashLine));
        }

        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto &vertex = vertices[i];
            if (vertex.id == -1) continue;

            double vertexRadius = 12.0;
            CelestialObject *obj = (*celestialObjectsPtr)[i];

            if (obj->getType() == "Nebula") {
                Nebula *nebula = dynamic_cast<Nebula *>(obj);
                if (nebula) {
                    Nebula::nebulaType nType = nebula->getNebulaType();
                    switch (nType) {
                        case Nebula::nebulaType::Planetary:  vertexRadius = 12.0; break;
                        case Nebula::nebulaType::Supernova:  vertexRadius = 14.0; break;
                        case Nebula::nebulaType::Reflection: vertexRadius = 16.0; break;
                        case Nebula::nebulaType::Emission:   vertexRadius = 18.0; break;
                        case Nebula::nebulaType::Dark:       vertexRadius = 20.0; break;
                        default: vertexRadius = 15.0;
                    }
                }
            }
            else if (obj->getType() == "StarSystem") {
                StarSystem *system = dynamic_cast<StarSystem *>(obj);
                if (system) {
                    Star::starType sType = system->getStar().getStarType();
                    switch (sType) {
                        case Star::starType::Neutron_Star:       vertexRadius = 3.0; break;
                        case Star::starType::White_Dwarf:        vertexRadius = 3.5; break;
                        case Star::starType::Brown_Dwarf:        vertexRadius = 4.0; break;
                        case Star::starType::Red_Dwarf:          vertexRadius = 4.5; break;
                        case Star::starType::Main_sequence_Star: vertexRadius = 5.5; break;
                        case Star::starType::Red_Giant:          vertexRadius = 7.0; break;
                        default: vertexRadius = 5.0;
                    }
                }
            }

            QPointF vCenter(vertex.x, vertex.y);

            if (i < celestialObjectsPtr->size() && (*celestialObjectsPtr)[i]) {
                CelestialObject *obj = (*celestialObjectsPtr)[i];

                if (obj->getType() == "StarSystem") {
                     StarSystem *system = dynamic_cast<StarSystem *>(obj);
                     if (system) {
                        Star::starType type = system->getStar().getStarType();
                        if (type == Star::starType::White_Dwarf) {
                            painter.setBrush(Qt::lightGray); painter.setPen(QPen(Qt::darkMagenta, 2));
                        } else if (type == Star::starType::Brown_Dwarf) {
                            painter.setBrush(Qt::color1); painter.setPen(QPen(Qt::white, 2));
                        } else if (type == Star::starType::Main_sequence_Star) {
                            painter.setBrush(Qt::yellow); painter.setPen(QPen(Qt::color0, 2));
                        } else if (type == Star::starType::Neutron_Star) {
                            painter.setBrush(Qt::darkBlue); painter.setPen(QPen(Qt::white, 2));
                        } else if (type == Star::starType::Red_Giant) {
                            painter.setBrush(Qt::red); painter.setPen(QPen(Qt::darkRed, 2));
                        } else if (type == Star::starType::Red_Dwarf) {
                            painter.setBrush(Qt::green); painter.setPen(QPen(Qt::red, 2));
                        } else {
                            painter.setBrush(Qt::white); painter.setPen(QPen(Qt::gray, 2));
                        }
                    }
                } else if (obj->getType() == "Nebula") {
                    Nebula *nebula = dynamic_cast<Nebula *>(obj);
                    if (nebula) {
                        QRadialGradient gradient(vCenter, vertexRadius);
                        Nebula::nebulaType type = nebula->getNebulaType();

                        painter.setPen(Qt::NoPen);

                        if (type == Nebula::nebulaType::Planetary) {
                            gradient.setColorAt(0, QColor(100, 255, 255, 140));
                            gradient.setColorAt(0.5, QColor(50, 150, 255, 80));
                            gradient.setColorAt(1, QColor(20, 50, 150, 0));
                        } else if (type == Nebula::nebulaType::Supernova) {
                            gradient.setColorAt(0, QColor(255, 255, 150, 150));
                            gradient.setColorAt(0.4, QColor(255, 100, 50, 90));
                            gradient.setColorAt(1, QColor(150, 0, 0, 0));
                        } else if (type == Nebula::nebulaType::Emission) {
                            gradient.setColorAt(0, QColor(255, 100, 150, 140));
                            gradient.setColorAt(0.5, QColor(200, 50, 100, 80));
                            gradient.setColorAt(1, QColor(100, 20, 50, 0));
                        } else if (type == Nebula::nebulaType::Reflection) {
                            gradient.setColorAt(0, QColor(150, 200, 255, 140));
                            gradient.setColorAt(0.5, QColor(80, 120, 200, 80));
                            gradient.setColorAt(1, QColor(30, 50, 120, 0));
                        } else if (type == Nebula::nebulaType::Dark) {
                            gradient.setColorAt(0, QColor(80, 60, 100, 180));
                            gradient.setColorAt(0.5, QColor(50, 30, 70, 100));
                            gradient.setColorAt(1, QColor(20, 10, 30, 0));
                        } else {
                            gradient.setColorAt(0, QColor(200, 200, 255, 140));
                            gradient.setColorAt(1, QColor(50, 50, 100, 0));
                        }

                        painter.setBrush(gradient);

                        RandomGenerator rng(vertex.id * 9999);

                        int starCount = static_cast<int>(vertexRadius * 2.5);

                        for (int s = 0; s < starCount; ++s) {
                            double angle = rng.getDouble(0, 2 * M_PI);

                            double r = std::sqrt(rng.getDouble(0.0, 1.0)) * (vertexRadius * 0.9);

                            double sx = vCenter.x() + r * std::cos(angle);
                            double sy = vCenter.y() + r * std::sin(angle);

                            if (s % 7 == 0) {
                                painter.setPen(QPen(QColor(255, 255, 255, 220), 1.5));
                            } else {
                                painter.setPen(QPen(QColor(255, 255, 255, 140), 1.0));
                            }
                            painter.drawPoint(QPointF(sx, sy));
                        }

                        painter.setPen(Qt::NoPen);
                    }
                } else {
                    painter.setBrush(Qt::cyan);
                    painter.setPen(QPen(Qt::black, 2));
                }
            } else {
                painter.setBrush(Qt::cyan);
                painter.setPen(QPen(Qt::black, 2));
            }

            painter.drawEllipse(vCenter, vertexRadius, vertexRadius);

            if (highlightedIds.count(vertex.id)) {
                painter.setPen(Qt::white);
                QFont nameFont = painter.font();
                nameFont.setPointSize(10);
                nameFont.setBold(true);
                painter.setFont(nameFont);

                painter.drawText(QPointF(vertex.x + vertexRadius + 5, vertex.y - 5), vertex.name);
            }
         }
    }
}