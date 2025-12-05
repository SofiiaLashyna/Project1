#include "GraphWidget.h"
#include <QtGui/QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <RandomUtilities.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent) {
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        if (isDetailMode) {
            this->update();
        }
    });
    generateBackgroundStars();
}

void GraphWidget::generateBackgroundStars() {
    stars.clear();
    RandomGenerator rng;

    int starCount = 200;

    for (int i = 0; i < starCount; ++i) {
        BackgroundStar star;
        star.pos = QPointF(rng.getDouble(0.0, 1.0), rng.getDouble(0.0, 1.0));

        star.alpha = rng.getInt(50, 150);

        star.size = (rng.getInt(0, 10) > 8) ? 2.0 : 1.0;

        stars.push_back(star);
    }
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

        if (isDetailMode && !isPlanetMode && detailedVertexId != -1) {
            CelestialObject *obj = (*celestialObjectsPtr)[detailedVertexId];

            if (obj->getType() == "StarSystem") {
                StarSystem *system = dynamic_cast<StarSystem *>(obj);
                if (system) {
                    double starRadius = 24.0 * 3.0; // Врахування масштабу
                    // Використовуємо новий метод (але передаємо базовий радіус без скейлу, тому ділимо на 3)
                    double orbitStart = calculateOrbitStart(system, 24.0);

                    QPointF center(width() / 2.0, height() / 2.0);
                    double effectiveScale = 3.0; // Масштаб, який використовується в paintEvent

                    for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                        Planet *planet = system->getPlanets()[i];

                        // Отримуємо зміщення через новий метод
                        QPointF offset = calculatePlanetOffset(planet, i, system->getPlanets().size(), orbitStart);

                        // Координати планети на екрані = Центр + (Зміщення * Масштаб)
                        QPointF planetPos = center + (offset * effectiveScale);

                        double clickRadius = 30.0; // Допуск для кліку
                        double dx = clickPt.x() - planetPos.x();
                        double dy = clickPt.y() - planetPos.y();

                        if (dx * dx + dy * dy <= clickRadius * clickRadius) {
                            zoomToPlanet(i);
                            emit planetDoubleClicked(i);
                            return;
                        }
                    }
                }
            }
        } else {
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
    }
    QWidget::mouseDoubleClickEvent(event);
}

void GraphWidget::zoomToPlanet(int planetIndex) {
    isPlanetMode = true;
    detailedPlanetIndex = planetIndex;
    update();
}

void GraphWidget::resetPlanetZoom() {
    isPlanetMode = false;
    detailedPlanetIndex = -1;
    update();
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

void GraphWidget::setHighlightedNodes(const std::vector<int> &ids) {
    highlightedIds.clear();
    for (int id: ids) {
        highlightedIds.insert(id);
    }
    update();
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    if (vertices.empty()) return;

    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);

    for (const auto &star: stars) {
        double x = star.pos.x() * width();
        double y = star.pos.y() * height();

        painter.setBrush(QColor(255, 255, 255, star.alpha));
        painter.drawEllipse(QPointF(x, y), star.size, star.size);
    }

    if (!celestialObjectsPtr) return;

    if (isPlanetMode && isDetailMode && detailedVertexId != -1) {
        CelestialObject *obj = (*celestialObjectsPtr)[detailedVertexId];
        StarSystem *system = dynamic_cast<StarSystem *>(obj);

        if (system && detailedPlanetIndex >= 0 && detailedPlanetIndex < system->getPlanets().size()) {
            Planet *planet = system->getPlanets()[detailedPlanetIndex];

            // Малюємо ОДНУ велику планету по центру
            double centerX = width() / 2.0;
            double centerY = height() / 2.0;
            double planetRadius = 150.0; // Велика планета

            // Градієнт для атмосфери/об'єму
            QRadialGradient grad(centerX - 40, centerY - 40, planetRadius);
            grad.setColorAt(0, planet->getColor().lighter(120));
            grad.setColorAt(1, planet->getColor().darker(150));

            painter.setPen(Qt::NoPen);
            painter.setBrush(grad);
            painter.drawEllipse(QPointF(centerX, centerY), planetRadius, planetRadius);
            return;
        }
    }
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
                        painter.setPen(QPen(QColor(255, 255, 255, 220), 1.5));
                    } else {
                        painter.setPen(QPen(QColor(255, 255, 255, 140), 1.0));
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
            QColor starColor;
            if (system) {
                Star::starType type = system->getStar().getStarType();
                if (type == Star::starType::White_Dwarf) {
                    starColor = Qt::darkMagenta;
                } else if (type == Star::starType::Brown_Dwarf) {
                    starColor = Qt::white;
                } else if (type == Star::starType::Main_sequence_Star) {
                    starColor = Qt::yellow;
                } else if (type == Star::starType::Neutron_Star) {
                    starColor = Qt::darkBlue;
                } else if (type == Star::starType::Red_Giant) {
                    starColor = Qt::red;
                } else if (type == Star::starType::Red_Dwarf) {
                    starColor = Qt::green;
                } else {
                    starColor = Qt::white;
                }

                painter.setPen(Qt::NoPen);

                double glowRadius = starRadius * 2.5;
                QRadialGradient glow(center, glowRadius);


                glow.setColorAt(0, QColor(starColor.red(), starColor.green(), starColor.blue(), 180));

                glow.setColorAt(0.4, QColor(starColor.red(), starColor.green(), starColor.blue(), 80));

                glow.setColorAt(1, QColor(starColor.red(), starColor.green(), starColor.blue(), 0));

                painter.setBrush(glow);
                painter.drawEllipse(center, glowRadius, glowRadius);

                painter.setBrush(starColor);

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

                // double minMass = std::numeric_limits<double>::max();
                // double maxMass = std::numeric_limits<double>::lowest();
                //
                // for (const auto &p: system->getPlanets()) {
                //     minMass = std::min(minMass, p->getMass());
                //     maxMass = std::max(maxMass, p->getMass());
                // }
                // if (maxMass == minMass) maxMass = minMass + 1.0;
                //
                // int maxPlanetRadius = 5;
                // for (const auto &p: system->getPlanets()) {
                //     double massNorm = (p->getMass() - minMass) / (maxMass - minMass);
                //     int planetRadius = int(4.0 + 8.0 * massNorm);
                //     maxPlanetRadius = std::max(maxPlanetRadius, planetRadius);
                // }

                double orbitStart = calculateOrbitStart(system, starRadius);

                static RandomGenerator rng;
                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet *planetPtr = system->getPlanets()[i];
                    Planet &planet = *planetPtr;
                    if (planet.getColor() == QColor(0, 0, 0) || planet.getColor() == QColor()) {
                        planet.setColor(rng.getRandomColor());
                    }
                }

                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet *planet = system->getPlanets()[i];

                    // Використовуємо новий метод!
                    QPointF offset = calculatePlanetOffset(planet, i, system->getPlanets().size(), orbitStart);

                    // Координати планети (в системі координат painter'а, яка вже зміщена до центру зірки)
                    QPointF planetCenter(focusedV.x + offset.x(), focusedV.y + offset.y());

                    // Малюємо орбіту
                    // Радіус орбіти - це довжина вектора зміщення
                    double currentOrbitRadius = std::sqrt(offset.x() * offset.x() + offset.y() * offset.y());

                    painter.setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(center, currentOrbitRadius, currentOrbitRadius);
                    painter.setPen(Qt::NoPen);

                    // Розрахунок розміру планети (залишаємо локально або теж можна винести)
                    // Для спрощення тут можна продублювати розрахунок маси або просто взяти radius2
                    // Щоб код був ідеальним, radius2 теж варто рахувати в calculatePlanetOffset або окремому методі,
                    // але для візуалізації можна залишити тут розрахунок розміру:
                    double minMass = std::numeric_limits<double>::max();
                    // Повторний пошук мас для радіуса (не критично)
                    double maxMass = std::numeric_limits<double>::lowest();
                    for (auto *p: system->getPlanets()) {
                        minMass = std::min(minMass, p->getMass());
                        maxMass = std::max(maxMass, p->getMass());
                    }
                    if (abs(maxMass - minMass) < 0.001) maxMass = minMass + 1.0;

                    double massNorm = (planet->getMass() - minMass) / (maxMass - minMass);
                    double radius2 = 4.0 + 8.0 * massNorm;

                    painter.setBrush(planet->getColor());
                    painter.drawEllipse(planetCenter, radius2, radius2);
                }
            }
        }
    } else {
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

            if (i >= celestialObjectsPtr->size() || !(*celestialObjectsPtr)[i]) continue;

            CelestialObject *obj = (*celestialObjectsPtr)[i];
            QPointF vCenter(vertex.x, vertex.y);
            double vertexRadius = 12.0;
            if (obj->getType() == "StarSystem") {
                StarSystem *system = dynamic_cast<StarSystem *>(obj);
                if (system) {
                    Star::starType sType = system->getStar().getStarType();
                    QColor starColor;

                    switch (sType) {
                        case Star::starType::Neutron_Star: vertexRadius = 3.0;
                            starColor = QColor(100, 100, 255);
                            break;
                        case Star::starType::White_Dwarf: vertexRadius = 3.5;
                            starColor = Qt::white;
                            break;
                        case Star::starType::Brown_Dwarf: vertexRadius = 4.0;
                            starColor = QColor(165, 42, 42);
                            break;
                        case Star::starType::Red_Dwarf: vertexRadius = 4.5;
                            starColor = QColor(255, 69, 0);
                            break;
                        case Star::starType::Main_sequence_Star: vertexRadius = 5.5;
                            starColor = QColor(255, 255, 200);
                            break;
                        case Star::starType::Red_Giant: vertexRadius = 7.0;
                            starColor = QColor(255, 50, 50);
                            break;
                        default: vertexRadius = 5.0;
                            starColor = Qt::yellow;
                    }

                    painter.setPen(Qt::NoPen);
                    double glowRadius = vertexRadius * 3.0;
                    QRadialGradient glow(vCenter, glowRadius);
                    glow.setColorAt(0, QColor(starColor.red(), starColor.green(), starColor.blue(), 150));
                    glow.setColorAt(1, QColor(starColor.red(), starColor.green(), starColor.blue(), 0));

                    painter.setBrush(glow);
                    painter.drawEllipse(vCenter, glowRadius, glowRadius);

                    painter.setBrush(starColor);
                    painter.drawEllipse(vCenter, vertexRadius, vertexRadius);
                }
            } else if (obj->getType() == "Nebula") {
                Nebula *nebula = dynamic_cast<Nebula *>(obj);
                if (nebula) {
                    Nebula::nebulaType nType = nebula->getNebulaType();
                    switch (nType) {
                        case Nebula::nebulaType::Planetary: vertexRadius = 12.0;
                            break;
                        case Nebula::nebulaType::Supernova: vertexRadius = 14.0;
                            break;
                        case Nebula::nebulaType::Reflection: vertexRadius = 16.0;
                            break;
                        case Nebula::nebulaType::Emission: vertexRadius = 18.0;
                            break;
                        case Nebula::nebulaType::Dark: vertexRadius = 20.0;
                            break;
                        default: vertexRadius = 15.0;
                    }

                    QRadialGradient gradient(vCenter, vertexRadius);
                    painter.setPen(Qt::NoPen);

                    if (nType == Nebula::nebulaType::Planetary) {
                        gradient.setColorAt(0, QColor(100, 255, 255, 140));
                        gradient.setColorAt(1, QColor(20, 50, 150, 0));
                    } else if (nType == Nebula::nebulaType::Supernova) {
                        gradient.setColorAt(0, QColor(255, 255, 150, 150));
                        gradient.setColorAt(1, QColor(150, 0, 0, 0));
                    } else if (nType == Nebula::nebulaType::Emission) {
                        gradient.setColorAt(0, QColor(255, 100, 150, 140));
                        gradient.setColorAt(1, QColor(100, 20, 50, 0));
                    } else if (nType == Nebula::nebulaType::Reflection) {
                        gradient.setColorAt(0, QColor(150, 200, 255, 140));
                        gradient.setColorAt(1, QColor(30, 50, 120, 0));
                    } else if (nType == Nebula::nebulaType::Dark) {
                        gradient.setColorAt(0, QColor(80, 60, 100, 180));
                        gradient.setColorAt(1, QColor(20, 10, 30, 0));
                    } else {
                        gradient.setColorAt(0, QColor(200, 200, 255, 140));
                        gradient.setColorAt(1, QColor(50, 50, 100, 0));
                    }

                    painter.setBrush(gradient);
                    painter.drawEllipse(vCenter, vertexRadius, vertexRadius);

                    RandomGenerator rng(vertex.id * 9999);
                    int starCount = static_cast<int>(vertexRadius * 2.5);
                    for (int s = 0; s < starCount; ++s) {
                        double angle = rng.getDouble(0, 2 * M_PI);
                        double r = std::sqrt(rng.getDouble(0.0, 1.0)) * (vertexRadius * 0.9);
                        double sx = vCenter.x() + r * std::cos(angle);
                        double sy = vCenter.y() + r * std::sin(angle);

                        if (s % 7 == 0) painter.setPen(QPen(QColor(255, 255, 255, 220), 1.5));
                        else painter.setPen(QPen(QColor(255, 255, 255, 140), 1.0));

                        painter.drawPoint(QPointF(sx, sy));
                    }
                    painter.setPen(Qt::NoPen);
                }
            } else {
                painter.setBrush(Qt::cyan);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawEllipse(vCenter, 5, 5);
            }

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

// GraphWidget.cpp
double GraphWidget::calculateOrbitStart(StarSystem *system, double starRadius) {
    double minMass = std::numeric_limits<double>::max();
    double maxMass = std::numeric_limits<double>::lowest();

    for (const auto &p: system->getPlanets()) {
        minMass = std::min(minMass, p->getMass());
        maxMass = std::max(maxMass, p->getMass());
    }
    if (std::abs(maxMass - minMass) < 0.001) maxMass = minMass + 1.0;

    int maxPlanetRadius = 5;
    for (const auto &p: system->getPlanets()) {
        double massNorm = (p->getMass() - minMass) / (maxMass - minMass);
        int pr = int(4.0 + 8.0 * massNorm);
        maxPlanetRadius = std::max(maxPlanetRadius, pr);
    }

    return starRadius + maxPlanetRadius + 12.0;
}

QPointF GraphWidget::calculatePlanetOffset(const Planet *planet, int index, int totalPlanets, double orbitStart) const {
    double orbitRadiusBase = orbitStart + 40.0 * (index / double(std::max(totalPlanets - 1, 1)));

    double time = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    double angularSpeed = 1.0 / std::sqrt(planet->getOrbit() + 1.0);
    double angle = time * angularSpeed * 0.5;

    return QPointF(orbitRadiusBase * std::cos(angle),
                   orbitRadiusBase * std::sin(angle));
}
