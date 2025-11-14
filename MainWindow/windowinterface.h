#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include <QWidget>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class WindowInterface; }
QT_END_NAMESPACE

class GalaxyView;
class GalaxyView3D;

class WindowInterface : public QWidget
{
    Q_OBJECT

public:
    explicit WindowInterface(QWidget *parent = nullptr);
    ~WindowInterface() override;

private:
    Ui::WindowInterface *ui;
    GalaxyView *galaxyView;
    GalaxyView3D *galaxyView3D;
    RandomGenerator rng;
    nlohmann::json data;
    bool dataLoaded = false;

    void loadJsonData();
};

#endif // WINDOWINTERFACE_H
