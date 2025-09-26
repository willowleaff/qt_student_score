#include "optimized_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OptimizedMainWindow w;
    w.show();
    return a.exec();
}
