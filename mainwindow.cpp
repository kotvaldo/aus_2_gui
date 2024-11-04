#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Models.h"
#include <QMessageBox>



//zadaj dva subory na nacitanie
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), database("", "")
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    resize(1024, 768);
    showMaximized();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deleteSelectedItem(QListWidget* listWidget) {
    QListWidgetItem* selectedItem = listWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Warning", "No item selected!");
        return;
    }

    int uid = selectedItem->data(Qt::UserRole).toInt();

    int ret = QMessageBox::question(this, "Delete Confirmation",
                                    "Are you sure you want to delete this item?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        bool success = false;

        if (type == ProgramType::Area) {
            success = database.deleteAreaRecord(uid);
        } else if (type == ProgramType::Nehnutelnosti) {
            success = database.deleteNehnutelnostRecord(uid);
        } else if (type == ProgramType::Parcely) {
            success = database.deleteParcelaRecord(uid);
        }

        if (success) {
            delete selectedItem;

            // Osvieženie zoznamu na základe aktuálneho typu
            if (type == ProgramType::Area) {
                RefreshListByArea(listWidget, database.allAreas());
            } else if (type == ProgramType::Nehnutelnosti) {
                RefreshListByNehnutelnosti(listWidget, database.allNehnutelnosti());
            } else if (type == ProgramType::Parcely) {
                RefreshListByParcel(listWidget, database.allParcely());
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete item from database.");
        }
    }
}


void MainWindow::RefreshListByNehnutelnosti(QListWidget* listWidget, const std::vector<Nehnutelnost*>& nehnutelnosti) {
    listWidget->clear();
    for (Nehnutelnost* nehnutelnost : nehnutelnosti) {
        QString itemText = QString("Nehnutelnost UID: %1").arg(nehnutelnost->uid);
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);

        item->setData(Qt::UserRole, nehnutelnost->uid);
        item->setData(Qt::UserRole + 1, QString::number(nehnutelnost->gps->x));
        item->setData(Qt::UserRole + 2, QString::number(nehnutelnost->gps->y));
        item->setData(Qt::UserRole + 3, nehnutelnost->supisneCislo);
        item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->popis));

        listWidget->addItem(item);
    }
}

void MainWindow::RefreshListByArea(QListWidget* listWidget, const std::vector<Area*>& areas) {
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
            item->setData(Qt::UserRole + 5, true);  // true znamená Nehnutelnost
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
            item->setData(Qt::UserRole + 5, false);  // false znamená Parcela
        }

        if (item != nullptr) {
            listWidget->addItem(item);
        }
    }
}


void MainWindow::RefreshListByParcel(QListWidget* listWidget, const std::vector<Parcela*>& parcely) {
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






void MainWindow::on_load_data_btn_clicked()
{

    database.reloadAllData();
    if (type == ProgramType::Area) {
        RefreshListByArea(ui->list_of_items, database.allAreas());
    } else if (type == ProgramType::Nehnutelnosti) {
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
    } else if (type == ProgramType::Parcely) {
        RefreshListByParcel(ui->list_of_items, database.allParcely());
    }
}


void MainWindow::on_add_parcel_btn_clicked()
{
    int x1 = ui->x1_parcels->text().toInt();
    int y1 = ui->y1_parcels->text().toInt();
    int x2 = ui->x2_parcels->text().toInt();
    int y2 = ui->y2_parcels->text().toInt();
    int number = ui->number_parcels->text().toInt();
    QString description = ui->desc_parcels->text();

    database.addParcela(x1, y1, x2, y2, number, description.toStdString());

}


void MainWindow::on_show_propert_btn_clicked()
{
    type = ProgramType::Nehnutelnosti;
    RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
}


void MainWindow::on_show_parcels_btn_clicked()
{
    type = ProgramType::Parcely;
    RefreshListByParcel(ui->list_of_items, database.allParcely());
}


void MainWindow::on_show_areas_btn_clicked()
{
    type = ProgramType::Area;
    RefreshListByArea(ui->list_of_items, database.allAreas());
}


void MainWindow::on_edit_selected_btn_clicked() {
    QListWidgetItem* selectedItem = ui->list_of_items->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Warning", "No item selected!");
        return;
    }

    int uid = selectedItem->data(Qt::UserRole).toInt();

    int newX1 = ui->new_x1->text().toInt();
    int newY1 = ui->new_y1->text().toInt();
    int newX2 = ui->new_x2->text().toInt();
    int newY2 = ui->new_y2->text().toInt();
    int newNumber = ui->new_number->text().toInt(); // Could be Supisne Cislo or Cislo Parcely
    QString newDesc = ui->new_desc->text();

    switch (type) {
    case ProgramType::Area: {
        bool isNehnutelnost = selectedItem->data(Qt::UserRole + 5).toBool();

        if (isNehnutelnost) {
            bool success = database.editNehnutelnost(uid, newX1, newY1, newX2, newY2, newNumber, newDesc.toStdString());
            if (success) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
            }
        } else {
            bool success = database.editParcela(uid, newX1, newY1, newX2, newY2, newNumber, newDesc.toStdString());
            if (success) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
            }
        }
        break;
    }
    case ProgramType::Nehnutelnosti: {
        bool success = database.editNehnutelnost(uid, newX1, newY1, newX2, newY2, newNumber, newDesc.toStdString());
        if (success) {
            RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
        }
        break;
    }
    case ProgramType::Parcely: {
        bool success = database.editParcela(uid, newX1, newY1, newX2, newY2, newNumber, newDesc.toStdString());
        if (success) {
            RefreshListByParcel(ui->list_of_items, database.allParcely());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
        }
        break;
    }
    default:
        QMessageBox::warning(this, "Error", "Unknown Program Type!");
        break;
    }
}


