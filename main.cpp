#include <QApplication>
#include "mainwindow.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

int main(int argc, char *argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtDumpMemoryLeaks();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    return 0;
}
///
