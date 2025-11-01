// #include "galaxyview.h"
// #include "ui_GalaxyView.h"
// #include <QApplication>
// // #include "FruchtermanReingoldLayout.h" // Закоментовано, якщо не використовується
// #include <QMenu>
// #include <QInputDialog>
// #include <QPushButton>
// #include <QTextEdit>
// #include <QResizeEvent>
// #include <QVBoxLayout>
// #include <QLabel>
// #include <QDebug>
//
// GalaxyView::GalaxyView(QWidget *parent) :
//     QWidget(parent), ui(new Ui::GalaxyView) {
//     ui->setupUi(this);
//
//     graphWidget = new GraphWidget(this);
//
//     if (ui->graphArea->layout() == nullptr) {
//         QVBoxLayout* layout = new QVBoxLayout(ui->graphArea);
//         layout->setContentsMargins(0, 0, 0, 0);
//     }
//
//     ui->graphArea->layout()->addWidget(graphWidget);
//
//     paramsButton = new QPushButton("⚙️ Parameters", this);
//     paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");
//
//     paramsWindow = new QWidget(this);
//     paramsWindow->setStyleSheet(
//         "background-color: #333; color: white; border: 1px solid #555; border-radius: 5px;"
//     );
//
//     QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
//     paramsLayout->setContentsMargins(10, 10, 10, 10);
//     paramsLayout->setSpacing(5);
//
//     QLabel *titleLabel = new QLabel("Galaxy Parameters: \n", paramsWindow);
//     titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 1px;");
//
//     QTextEdit *infoText = new QTextEdit(paramsWindow);
//     infoText->setObjectName("infoTextWidget");
//     infoText->setReadOnly(true);
//
//     infoText->setText("Galaxy not generated yet.");
//     infoText->setStyleSheet("background-color: #444; color: #eee; border: none;");
//
//     QPushButton* editButton = new QPushButton("✎ Edit..", paramsWindow);
//     this->editButton = editButton;
//     editButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");
//
//
//     paramsLayout->addWidget(titleLabel);
//     paramsLayout->addWidget(infoText);
//     paramsLayout->addWidget(editButton);
//     paramsWindow->setLayout(paramsLayout);
//
//     paramsButton->resize(150, 35);
//     paramsWindow->resize(250, 210);
//
//     connect(paramsButton, &QPushButton::clicked, this, &GalaxyView::on_paramsButton_clicked);
//     connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
//     paramsWindow->hide();
// }
//
// GalaxyView::~GalaxyView() {
//     if (galaxy) {
//         delete galaxy;
//         galaxy = nullptr;
//     }
//     delete ui;
// }
// void GalaxyView::on_paramsButton_clicked() {
//     if (paramsWindow->isVisible()) {
//         paramsWindow->hide();
//     } else {
//         paramsWindow->show();
//         paramsWindow->raise();
//     }
// }
//
// void GalaxyView::resizeEvent(QResizeEvent *event) {
//     QWidget::resizeEvent(event);
//
//     if (paramsButton && paramsWindow) {
//         int margin = 20;
//         int btn_x = width() - paramsButton->width() - margin;
//         int btn_y = margin;
//         paramsButton->move(btn_x, btn_y);
//
//         int win_x = width() - paramsWindow->width() - margin;
//         int win_y = btn_y + paramsButton->height() + 10; // 10px відступ від кнопки
//         paramsWindow->move(win_x, win_y);
//     }
// }
// void GalaxyView::generateAndDisplayGalaxy(const nlohmann::json& data, RandomGenerator& rng) {
//     qDebug() << "Generating and displaying galaxy in GalaxyView...";
//
//     if (galaxy) {
//         delete galaxy;
//         galaxy = nullptr;
//     }
//
//     std::string galaxyNameFile = "Unnamed";
//     if (data.contains("Galaxy") && data["Galaxy"].is_array() && !data["Galaxy"].empty()) {
//         galaxyNameFile = data["Galaxy"][0].get<std::string>();
//     }
//     std::string randomGalaxyName = rng.getRandomNameFromFile(galaxyNameFile);
//
//     galaxy = new Galaxy<GraphList<CelestialObject*>>(randomGalaxyName);
//
//     galaxy->generateGalaxy(data, rng);
//
//     qDebug() << "Galaxy successfully generated:" << QString::fromStdString(randomGalaxyName);
//
//     ui->galaxyNameLabel->setText(QString("Created Galaxy: ") + QString::fromStdString(randomGalaxyName));
//
//     std::vector<W_Vertex> vertices;
//     int nVerticesTotal = galaxy->getGraph().getVertices().size();
//
//     for (int i = 0; i < nVerticesTotal; ++i) {
//         const auto& v = galaxy->getGraph().getVertices()[i];
//         if (v.id != -1) {
//             vertices.push_back({
//                 i,
//                 0,
//                 0,
//                 QString::fromStdString(galaxy->getObject()[i]->getName())
//             });
//         }
//     }
//
//     std::vector<W_Edge> edges;
//     for (const auto &e : galaxy->getGraph().getEdges()) {
//         if (e.isActive()) {
//             edges.push_back({e.from, e.to, e.weight});
//         }
//     }
//     this->dataPtr = const_cast<nlohmann::json*>(&data);
//     this->rngPtr = &rng;
//
//     int n = vertices.size();
//     if (n > 0) {
//         int cx = 550;
//         int cy = 350;
//     int radius;
//         for(int j = 0; j < n; ++j) {
//             radius = rng.getDouble(150, 250);
//             vertices[j].x = cx + radius * std::cos(2 * M_PI * j / n);
//             vertices[j].y = cy + radius * std::sin(2 * M_PI * j / n);
//         }
//     }
//
//     if (graphWidget) {
//         graphWidget->setGraph(vertices, edges);
//         graphWidget->update();
//     }
//     updateParametersWindow();
// }
//
// void GalaxyView::updateParametersWindow() {
//     QTextEdit *infoText = paramsWindow->findChild<QTextEdit*>("infoTextWidget");
//
//     if (infoText && galaxy) {
//         infoText->setText(galaxy->getGalaxyParameters());
//     } else if (infoText) {
//         infoText->setText("Cannot load parameters: Galaxy object is missing.");
//     }
// }
//
//
//
//
// GalaxyEditDialog::GalaxyEditDialog(Galaxy<GraphList<CelestialObject*>>* g,
//                                    RandomGenerator* rng,
//                                    const nlohmann::json* data,
//                                    QWidget* parent)
//     : QDialog(parent), galaxy(g), rngPtr(rng), dataPtr(data) // Ініціалізуємо нові члени
// {
//     setWindowTitle("Edit Galaxy: " + QString::fromStdString(g->getName()));
//
//     QVBoxLayout* layout = new QVBoxLayout(this);
//
//     QLabel* nameLabel = new QLabel("Galaxy Name:", this);
//     nameEdit = new QLineEdit(QString::fromStdString(galaxy->getName()), this);
//
//     QPushButton* saveNameButton = new QPushButton("Save Name", this);
//
//     QHBoxLayout* nameLayout = new QHBoxLayout;
//     nameLayout->addWidget(nameEdit);
//     nameLayout->addWidget(saveNameButton);
//
//     layout->addWidget(nameLabel);
//     layout->addLayout(nameLayout);
//
//     layout->addWidget(new QWidget(this));
//
//     QLabel* addLabel = new QLabel("Add New Object:", this);
//     QPushButton* addStarSystem = new QPushButton("Add Star System", this);
//     QPushButton* addNebula = new QPushButton("Add Nebula", this);
//
//     layout->addWidget(addLabel);
//     layout->addWidget(addStarSystem);
//     layout->addWidget(addNebula);
//
//     layout->addWidget(new QWidget(this));
//
//     QPushButton* closeButton = new QPushButton("Close", this);
//     layout->addWidget(closeButton);
//
//     connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
//
//     connect(addStarSystem, &QPushButton::clicked, this, &GalaxyEditDialog::on_addStarSystem_clicked);
//     connect(addNebula, &QPushButton::clicked, this, &GalaxyEditDialog::on_addNebula_clicked);
//
//     connect(saveNameButton, &QPushButton::clicked, this, [this]() {
//         galaxy->setName(nameEdit->text().toStdString());
//         setWindowTitle("Edit Galaxy: " + nameEdit->text());
//     });
//
//
// }
//
// void GalaxyEditDialog::on_addStarSystem_clicked() {
//     if (!rngPtr || !dataPtr) {
//         qWarning() << "Generator or data not initialized!";
//         return;
//     }
//     AddStarSystemDialog dlg(*rngPtr, *dataPtr, this);
//
//     if (dlg.exec() == QDialog::Accepted) {
//         // Отримуємо новий об'єкт
//         int newId = galaxy->getGraph().getNextVertexId();
//         StarSystem* newSystem = dlg.getNewStarSystem(newId);
//
//         if (newSystem) {
//             galaxy->addObject(newSystem); // Додаємо до галактики
//
//             // Після додавання закриваємо діалог GalaxyEditDialog з accept()
//             accept();
//         }
//     }
// }
//
// StarSystem* AddStarSystemDialog::getNewStarSystem(int id) const {
//
//     Galaxy<GraphList<CelestialObject*>> temp_galaxy;
//     StarSystem* system = temp_galaxy.generateStarSystem(id, rng, data);
//
//     if (!system) return nullptr;
//
//     std::string newName = nameEdit->text().toStdString();
//     system->setName(newName);
//
//     std::string starTypeStr = starTypeComboBox->currentText().toStdString();
//     system->getStar().setStarType(starTypeStr); // якщо є такий метод
//
//
//     // 4. Повернути об'єкт
//     return system;
// }
//
// QString GalaxyEditDialog::getNewGalaxyName() const {
//     return nameEdit->text();
// }
//
// void GalaxyView::on_editButton_clicked() {
//     if (!galaxy) {
//         QMessageBox::warning(this, "Error", "Can't generate Galaxy!");
//         return;
//     }
//
//     // ПЕРЕДАЄМО rngPtr та dataPtr
//     GalaxyEditDialog dlg(galaxy, rngPtr, dataPtr, this);
//
//     if (dlg.exec() == QDialog::Accepted) {
//         // ...
//     }
// }
//
// Nebula* AddNebulaDialog::getNewNebula() const {
//
//     // 1. Створити тимчасовий об'єкт Galaxy
//     Galaxy<GraphList<CelestialObject*>> temp_galaxy;
//
//     // 2. Викликати існуючу функцію генерації
//     // (generateNebula в Galaxy.h також потрібно змінити, щоб вона повертала об'єкт,
//     // а не додавала його одразу).
//     Nebula* nebula = temp_galaxy.generateNebula(rng, data);
//
//     if (!nebula) return nullptr;
//
//     // 3. Змінити параметри
//
//     // Зміна назви (ручний ввід)
//     std::string newName = nameEdit->text().toStdString();
//     nebula->setName(newName);
//
//     // Зміна типу (ручний ввід)
//     std::string typeStr = nebulaTypeComboBox->currentText().toStdString();
//
//     Nebula::nebulaType nType;
//     // ... (повна логіка перетворення typeStr в nType) ...
//
//     nebula->setNebulaType(nType); // якщо є такий метод
//
//     // 4. Повернути об'єкт
//     return nebula;
// }

#include "galaxyview.h"
#include "ui_GalaxyView.h"
#include <QApplication>
#include <QInputDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

GalaxyView::GalaxyView(QWidget *parent) :
    QWidget(parent), ui(new Ui::GalaxyView) {
    ui->setupUi(this);

    graphWidget = new GraphWidget(this);

    if (ui->graphArea->layout() == nullptr) {
        QVBoxLayout* layout = new QVBoxLayout(ui->graphArea);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    ui->graphArea->layout()->addWidget(graphWidget);

    paramsButton = new QPushButton("⚙️ Parameters", this);
    paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");

    paramsWindow = new QWidget(this);
    paramsWindow->setStyleSheet(
        "background-color: #333; color: white; border: 1px solid #555; border-radius: 5px;"
    );

    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
    paramsLayout->setContentsMargins(10, 10, 10, 10);
    paramsLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel("Galaxy Parameters: \n", paramsWindow);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 1px;");

    QTextEdit *infoText = new QTextEdit(paramsWindow);
    infoText->setObjectName("infoTextWidget");
    infoText->setReadOnly(true);

    infoText->setText("Galaxy not generated yet.");
    infoText->setStyleSheet("background-color: #444; color: #eee; border: none;");

    QPushButton* editButton = new QPushButton("✎ Edit..", paramsWindow);
    this->editButton = editButton;
    editButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");


    paramsLayout->addWidget(titleLabel);
    paramsLayout->addWidget(infoText);
    paramsLayout->addWidget(editButton);
    paramsWindow->setLayout(paramsLayout);

    paramsButton->resize(150, 35);
    paramsWindow->resize(250, 210);

    connect(paramsButton, &QPushButton::clicked, this, &GalaxyView::on_paramsButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    paramsWindow->hide();
}

GalaxyView::~GalaxyView() {
    if (galaxy) {
        delete galaxy;
        galaxy = nullptr;
    }
    delete ui;
}
void GalaxyView::on_paramsButton_clicked() {
    if (paramsWindow->isVisible()) {
        paramsWindow->hide();
    } else {
        paramsWindow->show();
        paramsWindow->raise();
    }
}

void GalaxyView::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (paramsButton && paramsWindow) {
        int margin = 20;
        int btn_x = width() - paramsButton->width() - margin;
        int btn_y = margin;
        paramsButton->move(btn_x, btn_y);

        int win_x = width() - paramsWindow->width() - margin;
        int win_y = btn_y + paramsButton->height() + 10; // 10px відступ від кнопки
        paramsWindow->move(win_x, win_y);
    }
}

void GalaxyView::updateGraphDisplay() {
    if (!galaxy || !graphWidget) return;
    if (!rngPtr) {
        qDebug() << "Error: rngPtr is null. Cannot generate new coordinates.";
        return;
    }

    int currentPositionsSize = static_cast<int>(vertexPositions.size());
    int nVerticesTotal = static_cast<int>(galaxy->getObject().size());

    if (nVerticesTotal > currentPositionsSize) {

        for (int i = currentPositionsSize; i < nVerticesTotal; ++i) {

            int cx = 550;
            int cy = 350;
            double radius = rngPtr->getDouble(150, 250);

            double angle = rngPtr->getDouble(0, 2 * M_PI);

            QPointF newPos(
                cx + radius * std::cos(angle),
                cy + radius * std::sin(angle)
            );

            vertexPositions.push_back(newPos);
            qDebug() << "Generated position for new object at index" << i;
        }
    }
    std::vector<W_Vertex> vertices;

    for (int i = 0; i < nVerticesTotal; ++i) {
        const auto& v = galaxy->getGraph().getVertices()[i];
        if (v.getId() != -1) {
            QPointF pos = vertexPositions[i];

            vertices.push_back({
                v.getId(),
                static_cast<int>(pos.x()),
                static_cast<int>(pos.y()),
                QString::fromStdString(galaxy->getObject()[i]->getName())
            });
        }
    }

    std::vector<W_Edge> edges;
    for (const auto &e : galaxy->getGraph().getEdges()) {
        if (e.isActive()) {
            edges.push_back({e.from, e.to, e.weight});
        }
    }

    if (graphWidget) {
        graphWidget->setGraph(vertices, edges);
        graphWidget->update();
    }
}

void GalaxyView::generateAndDisplayGalaxy(const nlohmann::json& data, RandomGenerator& rng) {
    qDebug() << "Generating and displaying galaxy in GalaxyView...";

    if (galaxy) {
        delete galaxy;
        galaxy = nullptr;
    }

    std::string galaxyNameFile = "Unnamed";
    if (data.contains("Galaxy") && data["Galaxy"].is_array() && !data["Galaxy"].empty()) {
        galaxyNameFile = data["Galaxy"][0].get<std::string>();
    }
    std::string randomGalaxyName = rng.getRandomNameFromFile(galaxyNameFile);

    galaxy = new Galaxy<GraphList<CelestialObject*>>(randomGalaxyName);

    galaxy->generateGalaxy(data, rng);

    this->dataPtr = const_cast<nlohmann::json*>(&data);
    this->rngPtr = &rng;

    qDebug() << "Galaxy successfully generated:" << QString::fromStdString(randomGalaxyName);

    ui->galaxyNameLabel->setText(QString("Created Galaxy: ") + QString::fromStdString(randomGalaxyName));

    vertexPositions.clear();
    int nVerticesTotal = galaxy->getObject().size();

    int n_active = 0;
    for (int i = 0; i < nVerticesTotal; ++i) {
        const auto& v = galaxy->getGraph().getVertices()[i];
        if (v.getId() != -1) {

            int cx = 550;
            int cy = 350;
            double radius = rng.getDouble(150, 250);

            double angle = 2 * M_PI * n_active / nVerticesTotal;

            QPointF pos(
                cx + radius * std::cos(angle),
                cy + radius * std::sin(angle)
            );

            vertexPositions.push_back(pos); // Зберігаємо QPointF
            n_active++;
        }
    }
    updateGraphDisplay();
    updateParametersWindow();
}
void GalaxyView::updateParametersWindow() {
    QTextEdit *infoText = paramsWindow->findChild<QTextEdit*>("infoTextWidget");

    if (infoText && galaxy) {
        infoText->setText(galaxy->getGalaxyParameters());
    } else if (infoText) {
        infoText->setText("Cannot load parameters: Galaxy object is missing.");
    }
}


GalaxyEditDialog::GalaxyEditDialog(Galaxy<GraphList<CelestialObject*>>* g,
                                   RandomGenerator* rng,
                                   const nlohmann::json* data,
                                   QWidget* parent)
    : QDialog(parent), galaxy(g), rngPtr(rng), dataPtr(data)
{
    setWindowTitle("Edit Galaxy: " + QString::fromStdString(g->getName()));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* nameLabel = new QLabel("Galaxy Name:", this);
    nameEdit = new QLineEdit(QString::fromStdString(galaxy->getName()), this);

    QPushButton* saveNameButton = new QPushButton("Save Name", this);

    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameEdit);
    nameLayout->addWidget(saveNameButton);

    layout->addWidget(nameLabel);
    layout->addLayout(nameLayout);

    layout->addWidget(new QWidget(this));

    QLabel* addLabel = new QLabel("Add New Object:", this);
    QPushButton* addStarSystem = new QPushButton("Add Star System", this);
    QPushButton* addNebula = new QPushButton("Add Nebula", this);

    layout->addWidget(addLabel);
    layout->addWidget(addStarSystem);
    layout->addWidget(addNebula);

    layout->addWidget(new QWidget(this));

    QPushButton* closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(addStarSystem, &QPushButton::clicked, this, &GalaxyEditDialog::on_addStarSystem_clicked);
    connect(addNebula, &QPushButton::clicked, this, &GalaxyEditDialog::on_addNebula_clicked);

    connect(saveNameButton, &QPushButton::clicked, this, [this]() {
        galaxy->setName(nameEdit->text().toStdString());
        setWindowTitle("Edit Galaxy: " + nameEdit->text());
    });


}AddStarSystemDialog::AddStarSystemDialog(RandomGenerator& rngRef,
                                         const nlohmann::json& dataRef,
                                         QWidget* parent)
    : QDialog(parent), rng(rngRef), data(dataRef)
{
    setWindowTitle("Add Star System");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("System Name:", this));
    nameEdit = new QLineEdit("New Star System", this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Number of Planets:", this));
    planetCountSpinBox = new QSpinBox(this);
    planetCountSpinBox->setRange(0, 10);
    planetCountSpinBox->setValue(3);
    layout->addWidget(planetCountSpinBox);

    layout->addWidget(new QLabel("Star Type:", this));
    starTypeComboBox = new QComboBox(this);
    starTypeComboBox->addItem("Main_sequence_Star");
    starTypeComboBox->addItem("Red_Giant");
    starTypeComboBox->addItem("White_Dwarf");
    starTypeComboBox->addItem("Brown_Dwarf");
    starTypeComboBox->addItem("Neutron_Star");
    starTypeComboBox->addItem("Red_Dwarf");
    layout->addWidget(starTypeComboBox);

    QPushButton* okButton = new QPushButton("Add", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

AddNebulaDialog::AddNebulaDialog(RandomGenerator& rngRef,
                                 const nlohmann::json& dataRef,
                                 QWidget* parent)
    : QDialog(parent), rng(rngRef), data(dataRef)
{
    setWindowTitle("Add Nebula");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Nebula Name:", this));
    nameEdit = new QLineEdit("New Nebula", this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Nebula Type:", this));
    nebulaTypeComboBox = new QComboBox(this);
    nebulaTypeComboBox->addItem("Emission");
    nebulaTypeComboBox->addItem("Reflection");
    nebulaTypeComboBox->addItem("Dark");
    nebulaTypeComboBox->addItem("Supernova");
    nebulaTypeComboBox->addItem("Planetary");
    layout->addWidget(nebulaTypeComboBox);

    QPushButton* okButton = new QPushButton("Add", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void GalaxyEditDialog::on_addStarSystem_clicked() {
    if (!rngPtr || !dataPtr) {
        qWarning() << "Generator or data not initialized!";
        return;
    }
    AddStarSystemDialog dlg(*rngPtr, *dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        int newId = static_cast<int>(galaxy->getObject().size());
        StarSystem* newSystem = dlg.getNewStarSystem(newId);

        if (newSystem) {
            galaxy->addObject(newSystem);

            QDialog::accept();
        }
    }
}

void GalaxyEditDialog::on_addNebula_clicked() {
    if (!rngPtr || !dataPtr) {
        qWarning() << "Generator or data not initialized!";
        return;
    }
    AddNebulaDialog dlg(*rngPtr, *dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        Nebula* newNebula = dlg.getNewNebula();

        if (newNebula) {
            galaxy->addObject(newNebula);

            QDialog::accept();
        }
    }
}


QString GalaxyEditDialog::getNewGalaxyName() const {
    return nameEdit->text();
}


StarSystem* AddStarSystemDialog::getNewStarSystem(int id) const {

    Galaxy<GraphList<CelestialObject*>> temp_galaxy;

    StarSystem* system = temp_galaxy.generateStarSystem(id, rng, data);

    if (!system) return nullptr;

    std::string newName = nameEdit->text().toStdString();
    system->setName(newName);

    std::string starTypeStr = starTypeComboBox->currentText().toStdString();

    system->getStar().setStarType(starTypeStr);


    int targetPlanetCount = planetCountSpinBox->value();
    if (targetPlanetCount<system->getPlanets().size()) {
        int dif = system->getPlanets().size() - targetPlanetCount;
        for (auto i : dif) {

        }
    }

    if (targetPlanetCount != system->getPlanets().size()) {
        qWarning() << "WARNING: Planet count cannot be changed easily after random generation.";
    }


    return system;
}


Nebula* AddNebulaDialog::getNewNebula() const {

    Galaxy<GraphList<CelestialObject*>> temp_galaxy;

    Nebula* nebula = temp_galaxy.generateNebula(rng, data);

    if (!nebula) return nullptr;

    std::string newName = nameEdit->text().toStdString();
    nebula->setName(newName);

    std::string typeStr = nebulaTypeComboBox->currentText().toStdString();
    nebula->setNebulaType(typeStr);


    return nebula;
}



void GalaxyView::on_editButton_clicked() {
    if (!galaxy) {
        QMessageBox::warning(this, "Error", "Can't generate Galaxy!");
        return;
    }

    GalaxyEditDialog dlg(galaxy, rngPtr, dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        updateParametersWindow();
        ui->galaxyNameLabel->setText(QString("Created Galaxy: ") + dlg.getNewGalaxyName());

        qDebug() << "New object added. Reloading graph display is needed.";
        updateGraphDisplay();
    }
}