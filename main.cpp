#include "CMainDraftWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainDraftWindow w;
    w.show();

    return a.exec();
}
