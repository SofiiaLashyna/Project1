#include <QtWidgets/QApplication>
#include "MainWindow/windowinterface.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    WindowInterface w;
    w.show();
    return a.exec();
}

