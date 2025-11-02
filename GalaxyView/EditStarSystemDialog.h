#ifndef EDITSTARSYSTEMDIALOG_H
#define EDITSTARSYSTEMDIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QSpinBox>
#include <QComboBox>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "StarSystem.h"

class EditStarSystemDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditStarSystemDialog(StarSystem *system, RandomGenerator *rng, const nlohmann::json *data, QWidget *parent = nullptr);

    void saveChanges();

private:
    StarSystem *starSystem;
    QLineEdit *nameEdit;
    QSpinBox *planetCountSpinBox;
    QComboBox *starTypeComboBox;
    RandomGenerator *rngPtr;
    const nlohmann::json *dataPtr;

    void updatePlanetCount();
};

#endif //EDITSTARSYSTEMDIALOG_H
