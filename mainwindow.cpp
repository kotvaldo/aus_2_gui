#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    resize(800, 600);
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_test_clicked()
{
    ui->label_test->setText("Fock you");
}

