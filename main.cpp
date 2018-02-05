#include <QMessageBox>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    if(argc == 2)
    {
        QString str = QString::fromLocal8Bit(argv[1]);
        w.addToPlaylist(str);
    }

    return a.exec();
}
