#ifndef GALAXYEDITDIALOG_H
#define GALAXYEDITDIALOG_H
#include <qlineedit.h>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "Galaxy.h"
#include "GraphList.h"
#include "CelestialObject.h"
#include "AddNebulaDialog.h"
#include "AddStarSystemDialog.h"

class GalaxyEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit GalaxyEditDialog(Galaxy<GraphList<CelestialObject *> > *g,
                              RandomGenerator *rng,
                              const nlohmann::json *data,
                              QWidget *parent = nullptr);


    QString getNewGalaxyName() const;

    private slots:
        void on_addStarSystem_clicked();
    void on_addNebula_clicked();

    signals:
        void galaxyModified();

private:
    Galaxy<GraphList<CelestialObject *> > *galaxy;
    QLineEdit *nameEdit;

    RandomGenerator *rngPtr;
    const nlohmann::json *dataPtr;
};
#endif //GALAXYEDITDIALOG_H
