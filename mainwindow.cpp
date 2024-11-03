#include "mainwindow.h"
#include "./ui_mainwindow.h"
enum class ProgramType {
    Nehnutelnosti,
    Parcely,
    Area
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    auto type = ProgramType::Nehnutelnosti;

    resize(1024, 768); // Nastavenie špecifické pre Nehnutelnosti
    showMaximized();

    std::string = path

    switch (type) {
    case ProgramType::Nehnutelnosti:

        break;
    case ProgramType::Parcely:

        break;
    case ProgramType::Area:

        break;
    }
    resize(800, 600);
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

