#include <QApplication>
#include "core.h"
#include "reportcore.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a (argc, argv);

    CuteDesigner::Core core;

    return a.exec();
}
