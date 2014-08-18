#include <QApplication>

#include "console.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Console console;
    console.run(argc, argv);

    return a.exec();
}
