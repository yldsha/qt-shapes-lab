#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "shapedatabase.h"

class QGraphicsScene;
class QGraphicsView;
class QComboBox;
class QSpinBox;
class Shape;
class QTableView;
class QSqlTableModel;
class QGroupBox;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddShape();
    void onConnectShapes();
    void onDeleteShape();
    void onApplyFilter();
    void onResetFilter();

private:
    Shape *createShape(int typeIndex, int id, int w, int h, int vertices);
    ShapeDatabase db;
    void deleteShape(int id);
    QSpinBox  *idFromBox;
    QSpinBox  *idToBox;
    QComboBox *filterTypeBox;
    QSpinBox  *minCountBox;
    

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView  *view;
    QComboBox      *typeBox;
    QSpinBox       *widthBox;
    QSpinBox       *heightBox;
    QSpinBox       *verticesBox;
    QTableView     *tableView;
    QSqlTableModel *model;

    int nextId = 1;              
    QMap<int, Shape*> shapes;   
    QLabel *countersLabel;     // "Фигур: N · Связей: M" в строке состояния
    void updateCounters(); 
};

#endif // MAINWINDOW_H