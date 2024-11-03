#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Models.h"
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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void populateListWidget(QListWidget* listWidget, const std::vector<Nehnutelnost*>& nehnutelnosti) {
    listWidget->clear();

    for (Nehnutelnost* nehnutelnost : nehnutelnosti) {
        QString itemText = QString("Nehnutelnost UID: %1").arg(nehnutelnost->uid);
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);

        item->setData(Qt::UserRole, nehnutelnost->uid);                        // UID
        item->setData(Qt::UserRole + 1, QString::number(nehnutelnost->gps->x)); // GPS X
        item->setData(Qt::UserRole + 2, QString::number(nehnutelnost->gps->y)); // GPS Y
        item->setData(Qt::UserRole + 3, nehnutelnost->supisneCislo);           // Supisne Cislo
        item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->popis)); // Popis

        listWidget->addItem(item);
    }
}

void RefreshListByArea(QListWidget* listWidget, const std::vector<Area*>& areas) {
    listWidget->clear();

    for (Area* area : areas) {
        QString itemText;
        QListWidgetItem* item = nullptr;

        if (area->nehnutelnost != nullptr) {
            Nehnutelnost* nehnutelnost = area->nehnutelnost;
            itemText = QString("Nehnutelnost: UID %1").arg(nehnutelnost->uid);
            item = new QListWidgetItem(itemText, listWidget);
            item->setData(Qt::UserRole, nehnutelnost->uid);
            item->setData(Qt::UserRole + 1, nehnutelnost->gps->x);
            item->setData(Qt::UserRole + 2, nehnutelnost->gps->y);
            item->setData(Qt::UserRole + 3, nehnutelnost->supisneCislo);
            item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->popis));
        }
        else if (area->parcela != nullptr) {
            Parcela* parcela = area->parcela;
            itemText = QString("Parcela: UID %1").arg(parcela->uid);
            item = new QListWidgetItem(itemText, listWidget);
            item->setData(Qt::UserRole, parcela->uid);
            item->setData(Qt::UserRole + 1, parcela->gps->x);
            item->setData(Qt::UserRole + 2, parcela->gps->y);
            item->setData(Qt::UserRole + 3, parcela->cisloParcely);
            item->setData(Qt::UserRole + 4, QString::fromStdString(parcela->popis));
        }

        if (item != nullptr) {
            listWidget->addItem(item);
        }
    }
}



void RefreshListByParcel(QListWidget* listWidget, const std::vector<Parcela*>& parcely) {
    listWidget->clear();

    for (Parcela* parcela : parcely) {

        QString itemText = QString("Parcela: UID %1").arg(parcela->uid);

        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);
        item->setData(Qt::UserRole, parcela->uid);
        item->setData(Qt::UserRole + 1, parcela->gps->x);
        item->setData(Qt::UserRole + 2, parcela->gps->y);
        item->setData(Qt::UserRole + 3, parcela->cisloParcely);
        item->setData(Qt::UserRole + 4, QString::fromStdString(parcela->popis));

        listWidget->addItem(item);

    }
}

