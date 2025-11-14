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

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        if (dataLoaded) {
            galaxyView->generateAndDisplayGalaxy(data, rng);
            ui->stackedWidget->setCurrentWidget(galaxyView);
        } else {
            qDebug() << "Error: JSON data not loaded";
        }
    });

    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        if (dataLoaded) {
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

WindowInterface::~WindowInterface() {
    delete ui;
}
