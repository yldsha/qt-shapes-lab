#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shape.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    scene = new QGraphicsScene(0, 0, 800, 600, this);
    view  = new QGraphicsView(scene);

    // Панель управления
    typeBox = new QComboBox;
    typeBox->addItems({"Прямоугольник", "Эллипс", "Многоугольник"});

    widthBox = new QSpinBox;
    widthBox->setRange(20, 300);
    widthBox->setValue(100);

    heightBox = new QSpinBox;
    heightBox->setRange(20, 300);
    heightBox->setValue(70);

    verticesBox = new QSpinBox;
    verticesBox->setRange(3, 12);
    verticesBox->setValue(5);

    auto *addButton = new QPushButton("Добавить");

    auto *controls = new QHBoxLayout;
    controls->addWidget(new QLabel("Тип:"));
    controls->addWidget(typeBox);
    controls->addWidget(new QLabel("Ширина:"));
    controls->addWidget(widthBox);
    controls->addWidget(new QLabel("Высота:"));
    controls->addWidget(heightBox);
    controls->addWidget(new QLabel("Вершин:"));
    controls->addWidget(verticesBox);
    controls->addWidget(addButton);
    controls->addStretch();

    auto *layout = new QVBoxLayout;
    layout->addLayout(controls);
    layout->addWidget(view);

    auto *central = new QWidget;
    central->setLayout(layout);
    setCentralWidget(central);
    resize(900, 700);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddShape);
}

MainWindow::~MainWindow()
{
    delete ui;
}


Shape *MainWindow::createShape(int typeIndex, int id, int w, int h, int v)
{
    switch (typeIndex) {
    case 0:  return new RectShape(id, w, h);
    case 1:  return new EllipseShape(id, w, h);
    default: return new PolygonShape(id, w, h, v);
    }
}

void MainWindow::onAddShape()
{
    const int w = widthBox->value();
    const int h = heightBox->value();
    const int v = verticesBox->value();

    Shape *shape = createShape(typeBox->currentIndex(), nextId, w, h, v);
    shapes.insert(nextId, shape);
    nextId++;

    scene->addItem(shape);  

    auto *rng = QRandomGenerator::global();
    shape->setPos(rng->bounded(800 - w), rng->bounded(600 - h));
}