#include "windowinterface.h"
#include "ui_WindowInterface.h"
#include "galaxyview.h"
#include "galaxyview3d.h"

WindowInterface::WindowInterface(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::WindowInterface) {
    ui->setupUi(this);

    loadJsonData();

    galaxyView = new GalaxyView(this);
    galaxyView3D = new GalaxyView3D(this);

    ui->stackedWidget->addWidget(galaxyView);
    ui->stackedWidget->addWidget(galaxyView3D);

    generateMenuStars();
    setupStyle();

    backgroundTimer = new QTimer(this);
    connect(backgroundTimer, &QTimer::timeout, this, [this]() {
        offset += 0.5;
        update();
    });
    backgroundTimer->start(16);
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        if (dataLoaded) {
            isMenuMode = false;
            backgroundTimer->stop();
            update();

            galaxyView->generateAndDisplayGalaxy(data, rng);
            ui->stackedWidget->setCurrentWidget(galaxyView);
        } else {
            qDebug() << "Error: JSON data not loaded";
        }
    });

    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        if (dataLoaded) {
            isMenuMode = false;
            backgroundTimer->stop();
            update();

            galaxyView3D->generateAndDisplayGalaxy(data, rng);
            ui->stackedWidget->setCurrentWidget(galaxyView3D);
        } else {
            qDebug() << "Error: JSON data not loaded";
        }
    });
}

void WindowInterface::loadJsonData() {
    std::ifstream inFile("C:/Users/Prj/Project1/RandomGalaxy/CelestialObjects.json");
    if (!inFile.is_open()) {
        qDebug() << "Cannot open JSON file!";
        return;
    }

    try {
        inFile >> data;
        if (!data.contains("Stars") || !data["Stars"].is_array()) {
            qDebug() << "Stars key missing or not an array in JSON!";
            return;
        }
        dataLoaded = true;
        qDebug() << "JSON data loaded successfully.";
    } catch (const std::exception &e) {
        qDebug() << "JSON parsing error:" << e.what();
    }
}

void WindowInterface::generateMenuStars() {
    stars.clear();
    RandomGenerator r(12345);
    for (int i = 0; i < 300; ++i) {
        MenuStar s;
        s.pos = QPointF(r.getDouble(0.0, 1.0), r.getDouble(0.0, 1.0));
        s.size = (r.getInt(0, 10) > 8) ? r.getDouble(1.5, 2.5) : r.getDouble(0.5, 1.5);
        s.alpha = r.getInt(100, 255);
        s.speed = r.getDouble(0.2, 1.0);
        stars.push_back(s);
    }
}

void WindowInterface::setupStyle() {
    QString btnStyle =
        "QPushButton {"
        "  background-color: rgba(20, 20, 40, 180);"
        "  border: 2px solid #00aaff;"
        "  border-radius: 12px;"
        "  color: white;"
        "  font-family: 'Ravie';"
        "  font-size: 16px;"
        "  padding: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(0, 170, 255, 50);"
        "  border: 2px solid #ffffff;"
        "  color: #00ffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(0, 170, 255, 100);"
        "  border: 2px solid #0088cc;"
        "}";

    ui->pushButton->setStyleSheet(btnStyle);
    ui->pushButton_2->setStyleSheet(btnStyle);

    ui->stackedWidget->setStyleSheet("background: transparent;");
    ui->page->setStyleSheet("background: transparent;");

    if (ui->label) {
        auto *glow = new QGraphicsDropShadowEffect(this);
        glow->setBlurRadius(25);
        glow->setColor(QColor(0, 255, 255));
        glow->setOffset(0, 0);
        ui->label->setGraphicsEffect(glow);

        ui->label->setStyleSheet("background: transparent; color: white; font-family: 'Ravie'; font-size: 24px;");
        ui->label->setAlignment(Qt::AlignCenter);
    }

}

void WindowInterface::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::black);

    if (isMenuMode) {
        QRadialGradient planetGrad(width(), height(), 400);
        planetGrad.setColorAt(0, QColor(80, 20, 100, 100));
        planetGrad.setColorAt(1, QColor(0, 0, 0, 0));
        painter.setBrush(planetGrad);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(width(), height()), 400, 400);

        painter.setPen(Qt::NoPen);
        for (const auto& s : stars) {
            double x = std::fmod((s.pos.x() * width()) + (offset * s.speed), width());
            if (x < 0) x += width();
            double y = s.pos.y() * height();

            painter.setBrush(QColor(255, 255, 255, s.alpha));
            painter.drawEllipse(QPointF(x, y), s.size, s.size);
        }
    }
}
WindowInterface::~WindowInterface() {
    delete ui;
}
