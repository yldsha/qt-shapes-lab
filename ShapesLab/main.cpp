#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    // Обход бага Qt 5.14 на новых macOS: без этого окна прозрачные
    qputenv("QT_MAC_WANTS_LAYER", "1");
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
