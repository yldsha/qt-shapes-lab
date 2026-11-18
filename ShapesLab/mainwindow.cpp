#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Создаём объекты
    label = new QLabel("Нажато: 0");
    auto *button = new QPushButton("Нажми меня");

    // 2. Раскладываем: надпись сверху, кнопка под ней
    auto *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(button);

    auto *central = new QWidget;
    central->setLayout(layout);
    setCentralWidget(central);

    // 3. Когда кнопку нажмут, вызови onButtonClicked
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClicked()
{
    count++;
    label->setText("Нажато: " + QString::number(count));
}
