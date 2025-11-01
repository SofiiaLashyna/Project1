// #include "windowinterface.h"
// #include "ui_WindowInterface.h"
// #include <QDebug>
// #include "galaxyview.h"
//
// WindowInterface::WindowInterface(QWidget *parent) :
//     QWidget(parent),
// ui(new Ui::WindowInterface) {
//     ui->setupUi(this);
//
//     connect(ui->pushButton, &QPushButton::clicked, this, [this](){
//         qDebug() << "Generate Galaxy clicked!";
//         generateGalaxy();
//         GalaxyView *view = new GalaxyView(this);
//         view->show();
//     });
// }
//
// void WindowInterface::generateGalaxy() {
//     qDebug() << "Galaxy is being generated...";
// }
//
// WindowInterface::~WindowInterface() {
//     delete ui;
// }
#include "windowinterface.h"
#include "ui_WindowInterface.h"
#include "galaxyview.h"

WindowInterface::WindowInterface(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::WindowInterface)
{
    ui->setupUi(this);

    loadJsonData();

    galaxyView = new GalaxyView(this);

    ui->stackedWidget->addWidget(galaxyView);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        if (dataLoaded) {
            galaxyView->generateAndDisplayGalaxy(data,rng);

            ui->stackedWidget->setCurrentWidget(galaxyView);
        }else {
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
    } catch (const std::exception& e) {
        qDebug() << "JSON parsing error:" << e.what();
    }
}

WindowInterface::~WindowInterface()
{
    delete ui;
}
