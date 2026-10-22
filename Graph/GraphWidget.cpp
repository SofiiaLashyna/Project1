#include "GraphWidget.h"
#include <QtGui/QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <QPainterPath>
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
                    double painterScale = 3.0;
                    double starRadius = 24.0;
                    double orbitStart = calculateOrbitStart(system, starRadius);

                    double maxOrbitAU = 0.0;
                    for (auto* p : system->getPlanets()) {
                        if (p->getOrbit() > maxOrbitAU) maxOrbitAU = p->getOrbit();
                    }
                    if (maxOrbitAU < 0.001) maxOrbitAU = 1.0;

                    double physScreenLimit = std::min(width(), height()) / 2.0 - 40.0;
                    double localScreenLimit = physScreenLimit / painterScale;

                    double scaleFactor = (localScreenLimit - orbitStart) / maxOrbitAU;
                    if (scaleFactor < 0.1) scaleFactor = 0.1;
                    if (scaleFactor > 40.0) scaleFactor = 40.0;

                    QPointF screenCenter(width() / 2.0, height() / 2.0);

                    for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                        Planet *planet = system->getPlanets()[i];

                        QPointF localOffset = calculatePlanetOffset(planet, orbitStart, scaleFactor);

                        QPointF screenOffset(localOffset.x() * painterScale, localOffset.y() * painterScale);
                        QPointF planetPos = screenCenter + screenOffset;

                        double clickRadius = 25.0;
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

bool GraphWidget::isInPlanetMode() const { return isPlanetMode; }

int GraphWidget::getDetailedPlanetIndex() const { return detailedPlanetIndex; }


void GraphWidget::setShowAxialTilt(bool show) {
    showAxialTilt = show;
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

            double centerX = width() / 2.0;
            double centerY = height() / 2.0;
            double planetRadius = 150.0;

            unsigned int seed = detailedVertexId * 100 + detailedPlanetIndex;
            RandomGenerator planetRng(seed);

            bool hasRings = planet->hasRings();
            double ringInnerRadius = planetRadius * planet->getRingInnerFactor();
            double ringOuterRadius = planetRadius * planet->getRingOuterFactor();
            RGBColor rc = planet->getRingColor();
            QColor ringColor(rc.r, rc.g, rc.b, rc.a);
            double ringTilt = 0.3;
            double timeSec = QDateTime::currentMSecsSinceEpoch() / 1000.0;
            int ringCount = planetRng.getInt(6000, 15000);

            if (hasRings) {
                painter.setBrush(Qt::NoBrush);
                QColor baseColor = ringColor;
                baseColor.setAlpha(15);
                painter.setPen(QPen(baseColor, 2));

                for (double r = ringInnerRadius; r <= ringOuterRadius; r += 1.0) {
                    painter.drawArc(QRectF(centerX - r, centerY - r * ringTilt, r * 2, r * 2 * ringTilt),
                                    0 * 16, 180 * 16);
                }

                painter.setPen(QPen(QColor(ringColor.red(), ringColor.green(), ringColor.blue(), 60), 1));
                painter.drawArc(QRectF(centerX - ringInnerRadius, centerY - ringInnerRadius * ringTilt,
                                       ringInnerRadius * 2, ringInnerRadius * 2 * ringTilt), 0 * 16, 180 * 16);
                painter.drawArc(QRectF(centerX - ringOuterRadius, centerY - ringOuterRadius * ringTilt,
                                       ringOuterRadius * 2, ringOuterRadius * 2 * ringTilt), 0 * 16, 180 * 16);

                painter.setPen(Qt::NoPen);
                for (int i = 0; i < ringCount / 2; ++i) {
                    double angle = planetRng.getDouble(M_PI, 2 * M_PI);
                    double dist = planetRng.getDouble(ringInnerRadius, ringOuterRadius);
                    double wobble = std::sin(timeSec * 2.0 + i * 0.1) * 2.0;

                    double animatedDist = dist + wobble;

                    double px = centerX + animatedDist * std::cos(angle);
                    double py = centerY + animatedDist * std::sin(angle) * ringTilt;
                    int alpha = planetRng.getInt(20, 100);
                    painter.setBrush(QColor(ringColor.red(), ringColor.green(), ringColor.blue(), alpha));

                    double baseSize = 0.25;
                    double variance = std::pow(planetRng.getDouble(0.0, 1.0), 3.0) * 1.25;
                    double particleSize = baseSize + variance;

                    painter.drawEllipse(QPointF(px, py), particleSize, particleSize);
                }
            }
            if (planet->isHabitable()) {
                double atmRadius = planetRadius * 1.25;
                QRadialGradient atmGrad(centerX, centerY, atmRadius);
                atmGrad.setColorAt(0.5, QColor(100, 200, 255, 130));
                atmGrad.setColorAt(0.8, QColor(100, 200, 255, 30));
                atmGrad.setColorAt(1.0, Qt::transparent);

                painter.setBrush(atmGrad);
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(QPointF(centerX, centerY), atmRadius, atmRadius);
            }

            if (showAxialTilt) {
                double tiltAngle = planet->getInclination();
                if (std::abs(tiltAngle) > 0.01) {
                    painter.save();
                    painter.translate(centerX, centerY);

                    double axisLen = planetRadius * 1.6;
                    double arcRad = planetRadius * 1.2;

                    painter.setPen(QPen(QColor(255, 255, 255, 100), 1, Qt::DashLine));
                    painter.drawLine(QPointF(0, -axisLen), QPointF(0, axisLen));

                    painter.setPen(QPen(QColor(255, 255, 100, 150), 2));
                    painter.drawArc(QRectF(-arcRad, -arcRad, arcRad * 2, arcRad * 2),
                                    90 * 16, -tiltAngle * 16);

                    painter.setPen(QPen(QColor(255, 255, 255, 220)));
                    QFont font = painter.font();
                    font.setPixelSize(14);
                    font.setBold(true);
                    painter.setFont(font);
                    QString angleText = QString::number(tiltAngle, 'f', 2) + "°";
                    QRectF textRect(-50, -axisLen - 25, 100, 20);
                    painter.drawText(textRect, Qt::AlignCenter, angleText);

                    painter.rotate(tiltAngle);
                    QPen tiltPen(QColor(255, 255, 255, 200), 2, Qt::SolidLine);
                    painter.setPen(tiltPen);
                    painter.drawLine(QPointF(0, -axisLen), QPointF(0, axisLen));

                    painter.setBrush(Qt::white);
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(QPointF(0, -planetRadius), 3, 3);
                    painter.drawEllipse(QPointF(0, planetRadius), 3, 3);

                    painter.restore();
                }
            }

            // Конвертуємо наш доменний колір у QColor
            RGBColor pc = planet->getColor();
            QColor planetQColor(pc.r, pc.g, pc.b, pc.a);

            painter.setBrush(planetQColor);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(QPointF(centerX, centerY), planetRadius, planetRadius);

            painter.save();
            QPainterPath path;
            path.addEllipse(QPointF(centerX, centerY), planetRadius, planetRadius);
            painter.setClipPath(path);

            int spotsCount = planetRng.getInt(5, 12);
            for (int k = 0; k < spotsCount; ++k) {
                bool darker = planetRng.getBool();
                QColor spotColor = darker ? planetQColor.darker(140) : planetQColor.lighter(140);
                spotColor.setAlpha(180);

                painter.setBrush(spotColor);
                painter.setPen(Qt::NoPen);

                double spotX = centerX + planetRng.getDouble(-planetRadius * 0.75, planetRadius * 0.75);
                double spotY = centerY + planetRng.getDouble(-planetRadius * 0.75, planetRadius * 0.75);
                double spotW = planetRng.getDouble(planetRadius * 0.1, planetRadius * 0.3);
                double spotH = spotW * 0.6;

                painter.drawEllipse(QPointF(spotX, spotY), spotW, spotH);
            }
            painter.restore();

            QRadialGradient shadowGrad(centerX - 40, centerY - 40, planetRadius);

            shadowGrad.setColorAt(0.0, QColor(255, 255, 255, 30));

            shadowGrad.setColorAt(0.5, Qt::transparent);

            shadowGrad.setColorAt(1.0, QColor(0, 0, 0, 120));

            painter.setBrush(shadowGrad);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(QPointF(centerX, centerY), planetRadius, planetRadius);

            if (hasRings) {
                painter.setBrush(Qt::NoBrush);
                QColor baseColor = ringColor;
                baseColor.setAlpha(15);
                painter.setPen(QPen(baseColor, 2));

                for (double r = ringInnerRadius; r <= ringOuterRadius; r += 1.0) {
                    painter.drawArc(QRectF(centerX - r, centerY - r * ringTilt, r * 2, r * 2 * ringTilt),
                                    180 * 16, 180 * 16);
                }

                painter.setPen(QPen(QColor(ringColor.red(), ringColor.green(), ringColor.blue(), 60), 1));
                painter.drawArc(QRectF(centerX - ringInnerRadius, centerY - ringInnerRadius * ringTilt,
                                       ringInnerRadius * 2, ringInnerRadius * 2 * ringTilt), 180 * 16, 180 * 16);
                painter.drawArc(QRectF(centerX - ringOuterRadius, centerY - ringOuterRadius * ringTilt,
                                       ringOuterRadius * 2, ringOuterRadius * 2 * ringTilt), 180 * 16, 180 * 16);

                painter.setPen(Qt::NoPen);
                for (int i = 0; i < ringCount / 2; ++i) {
                    double angle = planetRng.getDouble(0, M_PI);
                    double dist = planetRng.getDouble(ringInnerRadius, ringOuterRadius);
                    double wobble = std::sin(timeSec * 2.0 + i * 0.1) * 2.0;
                    double animatedDist = dist + wobble;

                    double px = centerX + animatedDist * std::cos(angle);
                    double py = centerY + animatedDist * std::sin(angle) * ringTilt;
                    int alpha = planetRng.getInt(20, 100);
                    painter.setBrush(QColor(ringColor.red(), ringColor.green(), ringColor.blue(), alpha));

                    double baseSize = 0.25;
                    double variance = std::pow(planetRng.getDouble(0.0, 1.0), 3.0) * 1.25;
                    double particleSize = baseSize + variance;

                    painter.drawEllipse(QPointF(px, py), particleSize, particleSize);
                }
            }

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

                painter.setBrush(starColor);
                painter.drawEllipse(center, starRadius, starRadius);

                double painterScale = 3.0;

                double maxOrbitAU = 0.0;
                for (auto* p : system->getPlanets()) {
                    if (p->getOrbit() > maxOrbitAU) maxOrbitAU = p->getOrbit();
                }
                if (maxOrbitAU < 0.001) maxOrbitAU = 1.0;

                double physScreenLimit = std::min(width(), height()) / 2.0 - 40.0;
                double localScreenLimit = physScreenLimit / painterScale;

                double orbitStart = calculateOrbitStart(system, starRadius);

                double scaleFactor = (localScreenLimit - orbitStart) / maxOrbitAU;
                if (scaleFactor < 0.1) scaleFactor = 0.1;
                if (scaleFactor > 40.0) scaleFactor = 40.0;

                for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                    Planet *planet = system->getPlanets()[i];

                    QPointF offset = calculatePlanetOffset(planet, orbitStart, scaleFactor);
                    QPointF planetCenter(center.x() + offset.x(), center.y() + offset.y());

                    double orbitRadiusPx = std::sqrt(offset.x()*offset.x() + offset.y()*offset.y());
                    painter.setPen(QPen(QColor(255, 255, 255, 40), 0, Qt::DotLine));
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(center, orbitRadiusPx, orbitRadiusPx);
                    painter.setPen(Qt::NoPen);

                    double minMass = 0.0001;
                    double maxMass = 1000.0;
                    double massNorm = (planet->getMass() - minMass) / (maxMass - minMass);
                    if(massNorm > 1.0) massNorm = 1.0;
                    if(massNorm < 0.0) massNorm = 0.0;

                    double localRadius = 2.0 + 4.0 * massNorm;
                    RGBColor sysPc = planet->getColor();
                    painter.setBrush(QColor(sysPc.r, sysPc.g, sysPc.b, sysPc.a));
                    painter.drawEllipse(planetCenter, localRadius, localRadius);
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

QPointF GraphWidget::calculatePlanetOffset(const Planet *planet, double orbitStart, double scaleFactor) const {
    double orbitRadiusBase = orbitStart + (planet->getOrbit() * scaleFactor);
    double time = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    double angularSpeed = 1.0 / std::sqrt(planet->getOrbit() + 1.0);
    double angle = time * angularSpeed * 0.5;

    return QPointF(orbitRadiusBase * std::cos(angle),
                   orbitRadiusBase * std::sin(angle));
}
