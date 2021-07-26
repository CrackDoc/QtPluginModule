#include <QtWidgets/QApplication>
#include "QtPluginModule.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CQtPluginModule w;
    return a.exec();
}
