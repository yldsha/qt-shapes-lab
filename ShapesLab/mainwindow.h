#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class QGraphicsScene;
class QGraphicsView;
class QComboBox;
class QSpinBox;
class Shape;

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

private:
    Shape *createShape(int typeIndex, int id, int w, int h, int vertices);

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView  *view;
    QComboBox      *typeBox;
    QSpinBox       *widthBox;
    QSpinBox       *heightBox;
    QSpinBox       *verticesBox;

    int nextId = 1;              
    QMap<int, Shape*> shapes;    
};

#endif // MAINWINDOW_H