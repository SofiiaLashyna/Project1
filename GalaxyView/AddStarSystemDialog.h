#ifndef ADDSTARSYSTEMDIALOG_H
#define ADDSTARSYSTEMDIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QSpinBox>
#include <QComboBox>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "StarSystem.h"

class AddStarSystemDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddStarSystemDialog(RandomGenerator &rng, const nlohmann::json &data, QWidget *parent = nullptr);

    StarSystem *getNewStarSystem(int id) const;

private:
    QLineEdit *nameEdit;
    QSpinBox *planetCountSpinBox;
    QComboBox *starTypeComboBox;
    RandomGenerator &rng;
    const nlohmann::json &data;
};

#endif //ADDSTARSYSTEMDIALOG_H
