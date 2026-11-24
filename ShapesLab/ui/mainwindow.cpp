#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shape.h"
#include "connection.h"
#include "countdelegate.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QStatusBar>
#include <QTableView>
#include <QSqlTableModel>
#include <QSplitter>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QGroupBox>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.open("shapes.db");

    model = new QSqlTableModel(this);
    model->setTable("shapes_view");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Тип");
    model->setHeaderData(2, Qt::Horizontal, "Связи");
    model->setHeaderData(3, Qt::Horizontal, "Количество");

    tableView = new QTableView;
    tableView->setModel(model);
    tableView->resizeColumnsToContents();
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);   
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  
    tableView->setItemDelegateForColumn(3, new CountDelegate(tableView));  
    auto *header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);              // все столбцы делят ширину
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);   // ID — по содержимому
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableView->setColumnWidth(3, 110);
    

    //  Фильтр
    idFromBox = new QSpinBox;
    idFromBox->setRange(0, 9999);
    idToBox = new QSpinBox;
    idToBox->setRange(0, 9999);
    idToBox->setValue(9999);

    filterTypeBox = new QComboBox;
    filterTypeBox->addItems({"Все", "Прямоугольник", "Эллипс", "Многоугольник"});

    minCountBox = new QSpinBox;
    minCountBox->setRange(0, 9999);

    auto *idRange = new QHBoxLayout;          
    idRange->addWidget(idFromBox);
    idRange->addWidget(new QLabel("до"));
    idRange->addWidget(idToBox);

    auto *applyButton = new QPushButton("Применить");
    auto *resetButton = new QPushButton("Сбросить");
    auto *filterButtons = new QHBoxLayout;
    filterButtons->addWidget(applyButton);
    filterButtons->addWidget(resetButton);

    auto *form = new QFormLayout;          
    form->addRow("Номера от:", idRange);
    form->addRow("Тип:", filterTypeBox);
    form->addRow("Количество не меньше:", minCountBox);
    form->addRow(filterButtons);

    auto *filterBox = new QGroupBox("Фильтр"); 
    filterBox->setLayout(form);

    // Правая часть окна: фильтр сверху, таблица снизу
    auto *rightPanel = new QWidget;
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->addWidget(filterBox);
    rightLayout->addWidget(tableView);     

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
    verticesBox->setEnabled(false);

    auto *addButton = new QPushButton("Добавить");
    auto *connectButton = new QPushButton("Соединить выбранные");
    auto *deleteButton = new QPushButton("Удалить выбранную");

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
    controls->addWidget(connectButton);
    controls->addWidget(deleteButton);
    controls->addStretch();
    

    auto *layout = new QVBoxLayout;
    layout->addLayout(controls);
    auto *splitter = new QSplitter;
    splitter->addWidget(view);        // слева холст
    splitter->addWidget(rightPanel); 
    splitter->setSizes({650, 350});
    splitter->setStretchFactor(0, 3); // холст шире таблицы в 3 раза
    splitter->setStretchFactor(1, 1);
    layout->addWidget(splitter);

    auto *central = new QWidget;
    central->setLayout(layout);
    setCentralWidget(central);
    resize(900, 700);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddShape);
    connect(typeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                verticesBox->setEnabled(index == 2);
            });
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectShapes);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteShape);
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::onApplyFilter);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetFilter);
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
    db.insertShape(shape -> id(), shape -> typeName());
    db.updateTotalCount(shape -> typeName());
    model -> select();

    auto *rng = QRandomGenerator::global();
    shape->setPos(rng->bounded(800 - w), rng->bounded(600 - h));
}

void MainWindow::onConnectShapes()
{
    const QList<QGraphicsItem*> selected = scene->selectedItems();
    if (selected.size() != 2) {
        statusBar()->showMessage("Выделите ровно две фигуры (Cmd+клик)", 3000);
        return;
    }

    Shape *a = dynamic_cast<Shape*>(selected[0]);
    Shape *b = dynamic_cast<Shape*>(selected[1]);
    if (!a || !b)
        return;

    for (Connection *c : a->connections()) {
        if (c->getFrom() == b || c->getTo() == b) {
            statusBar()->showMessage("Эти фигуры уже связаны", 3000);
            return;
        }
    }
    auto *connection = new Connection(a, b);
    a->addConnection(connection);
    b->addConnection(connection);
    scene->addItem(connection);

    db.insertConnection(a -> id(), b -> id());
    model -> select();

}

void MainWindow::onDeleteShape()
{
    int id = -1;

    const QModelIndexList rows = tableView->selectionModel()->selectedRows();
    if (!rows.isEmpty()) {
        id = model->data(model->index(rows.first().row(), 0)).toInt();
    } else {
        for (QGraphicsItem *item : scene->selectedItems()) {
            if (Shape *shape = dynamic_cast<Shape*>(item)) {
                id = shape->id();
                break;
            }
        }
    }

    if (id == -1) {
        statusBar()->showMessage("Выберите фигуру в таблице или на холсте", 3000);
        return;
    }
    deleteShape(id);
}

void MainWindow::deleteShape(int id)
{
    Shape *shape = shapes.value(id, nullptr);
    if (!shape)
        return;

    // 1. Линии: отцепить от соседей, убрать со сцены, удалить
    const QList<Connection*> lines = shape->connections();  
    for (Connection *c : lines) {
        Shape *other = (c->getFrom() == shape) ? c->getTo() : c->getFrom();
        other->removeConnection(c);
        scene->removeItem(c);
        delete c;
    }

    // 2. Фигура: убрать с холста и из словаря
    const QString type = shape->typeName();   
    scene->removeItem(shape);
    shapes.remove(id);
    delete shape;

    // 3. База: удалить строку, пересчитать, обновить таблицу
    db.deleteShape(id);
    db.updateTotalCount(type);
    model->select();
}

void MainWindow::onApplyFilter()
{
    QStringList conditions;   // список условий, потом склеим через AND


    conditions << QString("id BETWEEN %1 AND %2")
                  .arg(idFromBox->value())
                  .arg(idToBox->value());

    if (filterTypeBox->currentIndex() > 0)
        conditions << QString("type = '%1'").arg(filterTypeBox->currentText());

    if (minCountBox -> value() > 0)
        conditions << QString("total_count >= %1").arg(minCountBox->value());

    model->setFilter(conditions.join(" AND "));
    model->select();
}

void MainWindow::onResetFilter()
{
    idFromBox->setValue(0);
    idToBox->setValue(9999);
    filterTypeBox->setCurrentIndex(0);   // пункт "Все"
    minCountBox->setValue(0);
    model->setFilter("");
    model->select();
}