#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    // Обход бага Qt 5.14 на новых macOS: без этого окна прозрачные
    qputenv("QT_MAC_WANTS_LAYER", "1");
#endif
    QApplication a(argc, argv);
        a.setStyleSheet(R"(
        QWidget        { font-size: 13px; }
        QMainWindow    { background: #f4f3f8; }

        QGroupBox {
            background: white;
            border: 1px solid #dcd8ea;
            border-radius: 10px;
            margin-top: 14px;
            padding: 12px 10px 10px 10px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 4px;
            color: #6b4fbb;
        }

        QPushButton {
            background: white;
            border: 1px solid #cfc8e6;
            border-radius: 6px;
            padding: 6px 12px;
        }
        QPushButton:hover   { background: #efeaff; }
        QPushButton:pressed { background: #ddd3ff; }

        QPushButton#primary {
            background: #7c5cd6;
            color: white;
            border: none;
            font-weight: bold;
        }
        QPushButton#primary:hover { background: #6a49c7; }

        QTableView {
            background: white;
            alternate-background-color: #f8f6fd;
            border: 1px solid #dcd8ea;
            border-radius: 8px;
            selection-background-color: #ddd3ff;
            selection-color: black;
        }
        QHeaderView::section {
            background: #efeaff;
            border: none;
            padding: 6px;
            font-weight: bold;
        }

        QGraphicsView { border: 1px solid #dcd8ea; border-radius: 8px; }
    )");
    MainWindow w;
    w.show();
    return a.exec();
}
