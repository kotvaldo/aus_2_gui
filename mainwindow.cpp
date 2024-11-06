#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Models.h"
#include <QMessageBox>



//zadaj dva subory na nacitanie
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), database("C:/Users/adams/Desktop/Nehnutelnosti.csv", "C:/Users/adams/Desktop/Parcely.csv")
, ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    resize(1024, 768);
    showMaximized();
    ui->comboBox_width_neh_1->addItem("N");
    ui->comboBox_width_neh_1->addItem("S");

    ui->comboBox_width_neh_2->addItem("N");
    ui->comboBox_width_neh_2->addItem("S");

    ui->comboBox_width_par->addItem("N");
    ui->comboBox_width_par->addItem("S");

    ui->comboBox_length_par->addItem("W");
    ui->comboBox_length_par->addItem("E");

    ui->comboBox_length_neh_1->addItem("W");
    ui->comboBox_length_neh_1->addItem("E");

    ui->comboBox_length_par_2->addItem("W");
    ui->comboBox_length_par_2->addItem("E");

    ui->comboBox_length_neh_2->addItem("W");
    ui->comboBox_length_neh_2->addItem("E");

    ui->comboBox_width_par_2->addItem("N");
    ui->comboBox_width_par_2->addItem("S");

    ui->comboBox_program_type->addItem("Nehnutelosti");
    ui->comboBox_program_type->addItem("Parcely");
    ui->comboBox_program_type->addItem("Area");

    ui->comboBox_edit->addItem("N");
    ui->comboBox_edit->addItem("S");

    ui->comboBox_find->addItem("N");
    ui->comboBox_find->addItem("S");

    ui->comboBox_edit_2->addItem("W");
    ui->comboBox_edit_2->addItem("E");

    ui->comboBox_find_2->addItem("W");
    ui->comboBox_find_2->addItem("E");

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
        QString itemText = QString("Nehnutelnost UID: %1, GPS: (%2, %3, %4, %5), Supisne Cislo: %6, Popis: %7")
        .arg(nehnutelnost->uid)
            .arg(nehnutelnost->gps->x)
            .arg(nehnutelnost->gps->y)
            .arg(nehnutelnost->gps->width)
            .arg(nehnutelnost->gps->length)
            .arg(nehnutelnost->supisneCislo)
            .arg(QString::fromStdString(nehnutelnost->popis));
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);

        item->setData(Qt::UserRole, nehnutelnost->uid);
        item->setData(Qt::UserRole + 1, QString::number(nehnutelnost->gps->x));
        item->setData(Qt::UserRole + 2, QString::number(nehnutelnost->gps->y));
        item->setData(Qt::UserRole + 3, nehnutelnost->supisneCislo);
        item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->popis));

        listWidget->addItem(item);
    }

    type = ProgramType::Nehnutelnosti;
    ui->label_count->setText("Počet nehnutelnosti: " + QString::number(nehnutelnosti.size()));
}


void MainWindow::RefreshListByArea(QListWidget* listWidget, const std::vector<Area*>& areas) {
    listWidget->clear();
    for (Area* area : areas) {
        QString itemText;
        QListWidgetItem* item = nullptr;

        if (area->nehnutelnost != nullptr) {
            Nehnutelnost* nehnutelnost = area->nehnutelnost;
            itemText = QString("Nehnutelnost UID: %1, GPS: (%2, %3, %4, %5), Supisne Cislo: %6, Popis: %7")
                           .arg(nehnutelnost->uid)
                           .arg(nehnutelnost->gps->x)
                           .arg(nehnutelnost->gps->y)
                           .arg(nehnutelnost->gps->width)
                           .arg(nehnutelnost->gps->length)
                           .arg(nehnutelnost->supisneCislo)
                           .arg(QString::fromStdString(nehnutelnost->popis));
            item = new QListWidgetItem(itemText, listWidget);
            item->setData(Qt::UserRole, nehnutelnost->uid);
            item->setData(Qt::UserRole + 1, nehnutelnost->gps->x);
            item->setData(Qt::UserRole + 2, nehnutelnost->gps->y);
            item->setData(Qt::UserRole + 3, nehnutelnost->supisneCislo);
            item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->popis));
            item->setData(Qt::UserRole + 5, true);  // true znamená Nehnutelnost
        } else if (area->parcela != nullptr) {
            Parcela* parcela = area->parcela;
            itemText = QString("Parcela UID: %1, GPS: (%2, %3, %4, %5), Cislo Parcely: %6, Popis: %7")
                           .arg(parcela->uid)
                           .arg(parcela->gps->x)
                           .arg(parcela->gps->y)
                           .arg(parcela->gps->width)
                           .arg(parcela->gps->length)
                           .arg(parcela->cisloParcely)
                           .arg(QString::fromStdString(parcela->popis));
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
    type = ProgramType::Area;
     ui->label_count->setText("Počet area: " + QString::number(areas.size()));
}

void MainWindow::RefreshListByParcel(QListWidget* listWidget, const std::vector<Parcela*>& parcely) {
    listWidget->clear();
    for (Parcela* parcela : parcely) {
        QString itemText = QString("Parcela UID: %1, GPS: (%2, %3, %4, %5), Cislo Parcely: %6, Popis: %7")
        .arg(parcela->uid)
            .arg(parcela->gps->x)
            .arg(parcela->gps->y)
            .arg(parcela->gps->width)
            .arg(parcela->gps->length)
            .arg(parcela->cisloParcely)
            .arg(QString::fromStdString(parcela->popis));
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);
        item->setData(Qt::UserRole, parcela->uid);
        item->setData(Qt::UserRole + 1, parcela->gps->x);
        item->setData(Qt::UserRole + 2, parcela->gps->y);
        item->setData(Qt::UserRole + 3, parcela->cisloParcely);
        item->setData(Qt::UserRole + 4, QString::fromStdString(parcela->popis));

        listWidget->addItem(item);
    }
    type = ProgramType::Parcely;
    ui->label_count->setText("Počet parciel: " + QString::number(parcely.size()));

}





void MainWindow::on_load_data_btn_clicked()
{
    if (type == ProgramType::Area) {
        RefreshListByArea(ui->list_of_items, database.allAreas());
    } else if (type == ProgramType::Nehnutelnosti) {
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
    } else if (type == ProgramType::Parcely) {
        RefreshListByParcel(ui->list_of_items, database.allParcely());
    }
}


void MainWindow::on_add_parcel_btn_clicked() {
    double x1 = ui->x1_parcels->text().toDouble();
    double y1 = ui->y1_parcels->text().toDouble();
    char width1 = ui->comboBox_width_par->currentText().toStdString()[0];
    char length1 = ui->comboBox_length_par->currentText().toStdString()[0];
    QString description = ui->desc_parcels->text();
    int number = ui->number_parcels->text().toInt();

    bool hasSecondCoordinates = !ui->x2_parcels->text().isEmpty() && !ui->y2_parcels->text().isEmpty();

    if (hasSecondCoordinates) {
        double x2 = ui->x2_parcels->text().toDouble();
        double y2 = ui->y2_parcels->text().toDouble();
        char width2 = ui->comboBox_width_par_2->currentText().toStdString()[0];
        char length2 = ui->comboBox_length_par_2->currentText().toStdString()[0];

        database.addParcela(x1, y1, width1, length1, x2, y2, width2, length2, number, description.toStdString());
    } else {
        database.addParcela(x1, y1, width1, length1, number, description.toStdString());
    }

    RefreshListByParcel(ui->list_of_items, database.allParcely());
    type = ProgramType::Parcely;
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

    double newX1 = ui->new_x1->text().toDouble();
    double newY1 = ui->new_y1->text().toDouble();
    char newWidth = ui->comboBox_edit->currentText().toStdString()[0];
    char newLength = ui->comboBox_edit_2->currentText().toStdString()[0];
    int newNumber = ui->new_number->text().toInt();
    QString newDesc = ui->new_desc->text();

    switch (type) {
    case ProgramType::Area: {
        bool isNehnutelnost = selectedItem->data(Qt::UserRole + 5).toBool();

        if (isNehnutelnost) {
            bool success = database.editNehnutelnost(uid, newX1, newY1, newWidth, newLength, newNumber, newDesc.toStdString());
            if (success) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
            }
        } else {
            bool success = database.editParcela(uid, newX1, newY1, newWidth, newLength, newNumber, newDesc.toStdString());
            if (success) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
            }
        }
        break;
    }
    case ProgramType::Nehnutelnosti: {
        bool success = database.editNehnutelnost(uid, newX1, newY1, newWidth, newLength, newNumber, newDesc.toStdString());
        if (success) {
            RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
        }
        break;
    }
    case ProgramType::Parcely: {
        bool success = database.editParcela(uid, newX1, newY1, newWidth, newLength, newNumber, newDesc.toStdString());
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



void MainWindow::on_add_property_btn_clicked() {
    double x1 = ui->x1_property->text().toDouble();
    double y1 = ui->y1_property->text().toDouble();
    QString description = ui->desc_property->text();
    int number = ui->number_property->text().toInt();

    char width1 = ui->comboBox_width_neh_1->currentText().toStdString()[0];
    char length1 = ui->comboBox_length_neh_1->currentText().toStdString()[0];

    bool hasSecondCoordinates = !ui->x2_property->text().isEmpty() && !ui->y2_property->text().isEmpty();

    if (hasSecondCoordinates) {
        double x2 = ui->x2_property->text().toDouble();
        double y2 = ui->y2_property->text().toDouble();
        char width2 = ui->comboBox_width_neh_2->currentText().toStdString()[0];
        char length2 = ui->comboBox_length_neh_2->currentText().toStdString()[0];

        database.addNehnutelnost(x1, y1, width1, length1, x2, y2, width2, length2, number, description.toStdString());
    } else {
        database.addNehnutelnost(x1, y1, width1, length1, number, description.toStdString());
    }
}



void MainWindow::on_refresh_btn_clicked()
{
    if (type == ProgramType::Area) {
        RefreshListByArea(ui->list_of_items, database.allAreas());
    } else if (type == ProgramType::Nehnutelnosti) {
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
    } else if (type == ProgramType::Parcely) {
        RefreshListByParcel(ui->list_of_items, database.allParcely());
    } else {
        QMessageBox::warning(this, "Error", "Unknown Program Type!");
    }
}



void MainWindow::on_clear_btn_clicked()
{
    database.clearAllData();
    if (type == ProgramType::Area) {
        RefreshListByArea(ui->list_of_items, database.allAreas());
    } else if (type == ProgramType::Nehnutelnosti) {
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
    } else if (type == ProgramType::Parcely) {
        RefreshListByParcel(ui->list_of_items, database.allParcely());
    } else {
        QMessageBox::warning(this, "Error", "Unknown Program Type!");
    }

}


void MainWindow::on_save_button_clicked()
{
    database.saveToFiles();
}


void MainWindow::on_delete_btn_clicked()
{
    this->deleteSelectedItem(ui->list_of_items);
}


void MainWindow::on_show_duplicities_btn_clicked() {
    double x1 = ui->search_x->text().toDouble();
    double y1 = ui->search_y->text().toDouble();
    bool hasSecondCoordinate = !(ui->search_x2->text().isEmpty() || ui->search_y2->text().isEmpty());
    double x2 = hasSecondCoordinate ? ui->search_x2->text().toDouble() : 0;
    double y2 = hasSecondCoordinate ? ui->search_y2->text().toDouble() : 0;
    char width1 = ui->comboBox_find->currentText().toStdString()[0];
    char length1 = ui->comboBox_find_2->currentText().toStdString()[0];

    if (type == ProgramType::Area) {
        if (hasSecondCoordinate) {
            RefreshListByArea(ui->list_of_items, database.findAreas(x1, y1, width1, length1, x2, y2, width1, length1));
        } else {
            RefreshListByArea(ui->list_of_items, database.findAreasOnly(x1, y1, width1, length1));
        }
    } else if (type == ProgramType::Nehnutelnosti) {
        if (hasSecondCoordinate) {
            RefreshListByNehnutelnosti(ui->list_of_items, database.findNehnutelnosti(x1, y1, width1, length1, x2, y2, width1, length1));
        } else {
            RefreshListByNehnutelnosti(ui->list_of_items, database.findNehnutelnostiOnly(x1, y1, width1, length1));
        }
    } else if (type == ProgramType::Parcely) {
        if (hasSecondCoordinate) {
            RefreshListByParcel(ui->list_of_items, database.findParcely(x1, y1, width1, length1, x2, y2, width1, length1));
        } else {
            RefreshListByParcel(ui->list_of_items, database.findParcelyOnly(x1, y1, width1, length1));
        }
    } else {
        QMessageBox::warning(this, "Error", "Unknown Program Type!");
    }

}


void MainWindow::on_pushButton_clicked()
{
    this->type = ProgramType::Nehnutelnosti;
}


void MainWindow::on_pushButton_2_clicked()
{
    this->type = ProgramType::Parcely;
}


void MainWindow::on_pushButton_3_clicked()
{
    this->type = ProgramType::Area;
}


void MainWindow::on_load_from_file_clicked()
{
    database.LoadDataFromFile();
    RefreshListByArea(ui->list_of_items, database.allAreas());
}


void MainWindow::on_list_of_items_itemClicked(QListWidgetItem *item) {
    this->ui->new_x1->setText(QString::number(item->data(Qt::UserRole + 1).toDouble()));
    this->ui->new_y1->setText(QString::number(item->data(Qt::UserRole + 2).toDouble()));

    this->ui->new_number->setText(item->data(Qt::UserRole + 3).toString());
    this->ui->new_desc->setText(item->data(Qt::UserRole + 4).toString());

    this->ui->search_x->setText(QString::number(item->data(Qt::UserRole + 1).toDouble()));
    this->ui->search_y->setText(QString::number(item->data(Qt::UserRole + 2).toDouble()));

    this->ui->comboBox_edit->setCurrentText(item->data(Qt::UserRole + 6).toString());
    this->ui->comboBox_edit_2->setCurrentText(item->data(Qt::UserRole + 7).toString());

    this->ui->comboBox_find->setCurrentText(item->data(Qt::UserRole + 6).toString());
    this->ui->comboBox_find_2->setCurrentText(item->data(Qt::UserRole + 7).toString());

}



void MainWindow::on_generate_btn_clicked()
{

    int countPar = ui->generate_count_par->text().toInt();
    int countNeh = ui->generate_count_nehm->text().toInt();
    double prekryv = ui->generate_prekryv->text().toDouble();
    database.generateRandomUnits(countPar, countNeh, prekryv);
    RefreshListByArea(ui->list_of_items, database.allAreas());
}


void MainWindow::on_comboBox_program_type_currentIndexChanged(int index)
{
    if(index == 0) {
        type = ProgramType::Nehnutelnosti;
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
    } else if(index == 1) {
        type = ProgramType::Parcely;
        RefreshListByParcel(ui->list_of_items, database.allParcely());
    } else if (index == 2) {
        type = ProgramType::Area;
        RefreshListByArea(ui->list_of_items, database.allAreas());
    }
}

