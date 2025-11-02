#ifndef ADDNEBULADIALOG_H
#define ADDNEBULADIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QComboBox>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "Nebula.h"

class AddNebulaDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddNebulaDialog(RandomGenerator &rng, const nlohmann::json &data, QWidget *parent = nullptr);

    Nebula *getNewNebula() const;

private:
    QLineEdit *nameEdit;
    QComboBox *nebulaTypeComboBox;
    RandomGenerator &rng;
    const nlohmann::json &data;
};

#endif //ADDNEBULADIALOG_H
