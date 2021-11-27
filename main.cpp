#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.create();

    QScreen * screen = QApplication::screens().at(0);

    w.move(screen->geometry().center() - w.rect().center());

    w.show();

    return a.exec();
}
