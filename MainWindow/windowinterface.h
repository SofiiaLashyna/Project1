#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class WindowInterface; }
QT_END_NAMESPACE

class WindowInterface : public QWidget {
Q_OBJECT

public:
    explicit WindowInterface(QWidget *parent = nullptr);
    ~WindowInterface() override;

private:
    Ui::WindowInterface *ui;
};


#endif //WINDOWINTERFACE_H
