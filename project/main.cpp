#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Dejavu Sans Mono",15));
    MainWindow w;
    w.show();
    return a.exec();
}
