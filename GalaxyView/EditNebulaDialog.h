#ifndef EDITNEBULADIALOG_H
#define EDITNEBULADIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QComboBox>
#include "Nebula.h"

class EditNebulaDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditNebulaDialog(Nebula *nebula, QWidget *parent = nullptr);

    void saveChanges();

private:
    Nebula *nebula;
    QLineEdit *nameEdit;
    QComboBox *nebulaTypeComboBox;
};

#endif //EDITNEBULADIALOG_H
