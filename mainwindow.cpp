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
    resize(1350, 780);

    ui->comboBox_program_type->addItem("Nehnutelosti");
    ui->comboBox_program_type->addItem("Parcely");
    ui->comboBox_program_type->addItem("Area");

    ui->comboBox_edit->addItem("N");
    ui->comboBox_edit->addItem("S");

    ui->comboBox_edit_2->addItem("W");
    ui->comboBox_edit_2->addItem("E");

    ui->comboBox_edit_4->addItem("N");
    ui->comboBox_edit_4->addItem("S");

    ui->comboBox_edit_5->addItem("W");
    ui->comboBox_edit_5->addItem("E");






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
            int uidArea = selectedItem->data(Qt::UserRole + 8).toInt();
            success = database.deleteAreaRecord(uidArea);
        } else if (type == ProgramType::Nehnutelnosti) {
            success = database.deleteNehnutelnostRecord(uid);
        } else if (type == ProgramType::Parcely) {
            success = database.deleteParcelaRecord(uid);
        }

        if (success) {
            delete selectedItem;

            if (type == ProgramType::Area) {
                ui->comboBox_program_type->setCurrentIndex(2);
            } else if (type == ProgramType::Nehnutelnosti) {
                ui->comboBox_program_type->setCurrentIndex(0);
            } else if (type == ProgramType::Parcely) {
                ui->comboBox_program_type->setCurrentIndex(1);
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete item from database.");
        }
    }
}



void MainWindow::RefreshListByNehnutelnosti(QListWidget* listWidget, const std::vector<Nehnutelnost*>& nehnutelnosti) {
    listWidget->clear();
    for (Nehnutelnost* nehnutelnost : nehnutelnosti) {
        QString itemText = QString("Nehnutelnost UID: %1, {GPS: (%2, %3, %4, %5), Supisne Cislo: %6, Popis: %7}")
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
        item->setData(Qt::UserRole + 6, QString(nehnutelnost->gps->width));
        item->setData(Qt::UserRole + 7, QString(nehnutelnost->gps->length));

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
            itemText = QString("Area ID: %1, {Nehnutelnost UID: %2, GPS: (%3, %4, %5, %6), Supisne Cislo: %7, Popis: %8}")
                           .arg(area->uid)  // ID oblasti
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
            item->setData(Qt::UserRole + 6, QString(nehnutelnost->gps->width));
            item->setData(Qt::UserRole + 7, QString(nehnutelnost->gps->length));
            item->setData(Qt::UserRole + 8, area->uid);

        } else if (area->parcela != nullptr) {
            Parcela* parcela = area->parcela;
            itemText = QString("Area ID: %1, {Parcela UID: %2, GPS: (%3, %4, %5, %6), Cislo Parcely: %7, Popis: %8}")
                           .arg(area->uid)  // ID oblasti
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
            item->setData(Qt::UserRole + 6, QString(parcela->gps->width));
            item->setData(Qt::UserRole + 7, QString(parcela->gps->length));
            item->setData(Qt::UserRole + 8, area->uid);

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
        QString itemText = QString("Parcela UID: %1, {GPS: (%2, %3, %4, %5), Cislo Parcely: %6, Popis: %7}")
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
        item->setData(Qt::UserRole + 6, QString(parcela->gps->width));
        item->setData(Qt::UserRole + 7, QString(parcela->gps->length));

        listWidget->addItem(item);
    }
    type = ProgramType::Parcely;
    ui->label_count->setText("Počet parciel: " + QString::number(parcely.size()));

}








void MainWindow::editing_parameters() {
    QListWidgetItem* selectedItem = ui->list_of_items->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Warning", "No item selected!");
        return;
    }

    int uid = selectedItem->data(Qt::UserRole).toInt();

    GPSParameters newCoords{
        ui->x1_edit->text().toDouble(),
        ui->y1_edit->text().toDouble(),
        ui->comboBox_edit->currentText().toStdString()[0],
        ui->comboBox_edit_2->currentText().toStdString()[0]
    };

    if (type == ProgramType::Area) {
        bool isNehnutelnost = selectedItem->data(Qt::UserRole + 5).toBool();
        if (isNehnutelnost) {
            NehnutelnostParameters newParams{
                ui->new_number->text().toInt(),
                ui->description->toPlainText().toStdString()
            };
            if (database.editNehnutelnost(uid, newCoords, newParams)) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
            }
        } else {
            ParcelaParameters newParams{
                ui->new_number->text().toInt(),
                ui->description->toPlainText().toStdString()
            };
            if (database.editParcela(uid, newCoords, newParams)) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
            }
        }
    } else if (type == ProgramType::Nehnutelnosti) {
        NehnutelnostParameters newParams{
            ui->new_number->text().toInt(),
            ui->description->toPlainText().toStdString()
        };
        if (database.editNehnutelnost(uid, newCoords, newParams)) {
            RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
        }
    } else if (type == ProgramType::Parcely) {
        ParcelaParameters newParams{
            ui->new_number->text().toInt(),
            ui->description->toPlainText().toStdString()
        };
        if (database.editParcela(uid, newCoords, newParams)) {
            RefreshListByParcel(ui->list_of_items, database.allParcely());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
        }
    }
}








void MainWindow::on_clear_btn_clicked()
{
    database.clearAllData();
    if (type == ProgramType::Area) {
        RefreshListByArea(ui->list_of_items, database.allAreas());
        ui->comboBox_program_type->setCurrentIndex(2);
    } else if (type == ProgramType::Nehnutelnosti) {
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
        ui->comboBox_program_type->setCurrentIndex(0);
    } else if (type == ProgramType::Parcely) {
        RefreshListByParcel(ui->list_of_items, database.allParcely());
        ui->comboBox_program_type->setCurrentIndex(1);
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


void MainWindow::on_generate_btn_clicked()
{

    int countPar = ui->generate_count_par->text().toInt();
    int countNeh = ui->generate_count_nehm->text().toInt();
    double prekryv = ui->generate_prekryv->text().toDouble();
    database.generateRandomUnits(countPar, countNeh, prekryv);
    RefreshListByArea(ui->list_of_items, database.allAreas());
    ui->comboBox_program_type->setCurrentIndex(2);
}

void MainWindow::on_comboBox_program_type_currentIndexChanged(int index)
{
    if (index == 0) {
        type = ProgramType::Nehnutelnosti;
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
    } else if (index == 1) {
        type = ProgramType::Parcely;
        RefreshListByParcel(ui->list_of_items, database.allParcely());
    } else if (index == 2) {
        type = ProgramType::Area;
        RefreshListByArea(ui->list_of_items, database.allAreas());
    }
}



void MainWindow::on_load_from_file_clicked()
{
    database.LoadDataFromFile();
    RefreshListByArea(ui->list_of_items, database.allAreas());
    type = ProgramType::Area;
    ui->comboBox_program_type->setCurrentIndex(2);

}

void MainWindow::on_list_of_items_itemClicked(QListWidgetItem *item)
{
    this->ui->x1_edit->setText(QString::number(item->data(Qt::UserRole + 1).toDouble()));
    this->ui->y1_edit->setText(QString::number(item->data(Qt::UserRole + 2).toDouble()));

    this->ui->new_number->setText(item->data(Qt::UserRole + 3).toString());
    this->ui->description->setText(item->data(Qt::UserRole + 4).toString());

    this->ui->comboBox_edit->setCurrentText(item->data(Qt::UserRole + 6).toString());
    this->ui->comboBox_edit_2->setCurrentText(item->data(Qt::UserRole + 7).toString());


}


void MainWindow::findProperties() {

     GPSParameters coords1{
        ui->x1_edit->text().toDouble(),
        ui->y1_edit->text().toDouble(),
        ui->comboBox_edit->currentText().toStdString()[0],
        ui->comboBox_edit_2->currentText().toStdString()[0]
    };

    bool hasSecondCoordinate = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    std::vector<Nehnutelnost*> results;

    if (hasSecondCoordinate) {
        GPSParameters coords2{
            ui->x2_edit->text().toDouble(),
            ui->y2_edit->text().toDouble(),
            ui->comboBox_edit->currentText().toStdString()[0],
            ui->comboBox_edit_2->currentText().toStdString()[0]
        };

        BoundingBox bbox{coords1, coords2};
        results = database.findNehnutelnosti(bbox);
    } else {
        results = database.findNehnutelnostiOnly(coords1);
    }

    RefreshListByNehnutelnosti(ui->list_of_items, results);
    ui->label_count->setText("Počet nájdených nehnuteľností: " + QString::number(results.size()));
}

void MainWindow::findAreas() {

    GPSParameters coords1{
        ui->x1_edit->text().toDouble(),
        ui->y1_edit->text().toDouble(),
        ui->comboBox_edit->currentText().toStdString()[0],
        ui->comboBox_edit_2->currentText().toStdString()[0]
    };

    bool hasSecondCoordinate = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    std::vector<Area*> results;

    if (hasSecondCoordinate) {
        GPSParameters coords2{
            ui->x2_edit->text().toDouble(),
            ui->y2_edit->text().toDouble(),
            ui->comboBox_edit->currentText().toStdString()[0],
            ui->comboBox_edit_2->currentText().toStdString()[0]
        };

        BoundingBox bbox{coords1, coords2};
        results = database.findAreas(bbox);
    } else {
        results = database.findAreasOnly(coords1);
    }

    RefreshListByArea(ui->list_of_items, results);
    ui->label_count->setText("Počet nájdených areas: " + QString::number(results.size()));
}

void MainWindow::findParcely() {

    GPSParameters coords1{
        ui->x1_edit->text().toDouble(),
        ui->y1_edit->text().toDouble(),
        ui->comboBox_edit->currentText().toStdString()[0],
        ui->comboBox_edit_2->currentText().toStdString()[0]
    };

    bool hasSecondCoordinate = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    std::vector<Parcela*> results;

    if (hasSecondCoordinate) {
        GPSParameters coords2{
            ui->x2_edit->text().toDouble(),
            ui->y2_edit->text().toDouble(),
            ui->comboBox_edit->currentText().toStdString()[0],
            ui->comboBox_edit_2->currentText().toStdString()[0]
        };

        BoundingBox bbox{coords1, coords2};
        results = database.findParcely(bbox);
    } else {
        results = database.findParcelyOnly(coords1);
    }

    RefreshListByParcel(ui->list_of_items, results);
    ui->label_count->setText("Počet nájdených parciel: " + QString::number(results.size()));
}

void MainWindow::addProperty() {
    GPSParameters coords1{
        ui->x1_edit->text().toDouble(),
        ui->y1_edit->text().toDouble(),
        ui->comboBox_edit->currentText().toStdString()[0],
        ui->comboBox_edit_2->currentText().toStdString()[0]
    };

    bool hasSecondCoordinates = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    NehnutelnostParameters params{
        ui->new_number->text().toInt(),
        ui->description->toPlainText().toStdString()
    };

    if (hasSecondCoordinates) {
        GPSParameters coords2{
            ui->x2_edit->text().toDouble(),
            ui->y2_edit->text().toDouble(),
            ui->comboBox_edit->currentText().toStdString()[0],
            ui->comboBox_edit_2->currentText().toStdString()[0]
        };

        BoundingBox bbox{coords1, coords2};
        database.addNehnutelnost(bbox, params);
    } else {
        database.addSingleNehnutelnost(coords1, params);
    }

}


void MainWindow::addParcel() {
    GPSParameters coords1{
        ui->x1_edit->text().toDouble(),
        ui->y1_edit->text().toDouble(),
        ui->comboBox_edit->currentText().toStdString()[0],
        ui->comboBox_edit_2->currentText().toStdString()[0]
    };

    bool hasSecondCoordinates = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    ParcelaParameters params{
        ui->new_number->text().toInt(),
        ui->description->toPlainText().toStdString()
    };

    if (hasSecondCoordinates) {
        GPSParameters coords2{
            ui->x2_edit->text().toDouble(),
            ui->y2_edit->text().toDouble(),
            ui->comboBox_edit->currentText().toStdString()[0],
            ui->comboBox_edit_2->currentText().toStdString()[0]
        };

        BoundingBox bbox{coords1, coords2};
        database.addParcela(bbox, params);
    } else {
        database.addSingleParcela(coords1, params);
    }

}



void MainWindow::on_task1_btn_clicked()
{

    ui->comboBox_program_type->setCurrentIndex(0);

    findProperties();

    type = ProgramType::Nehnutelnosti;

}


void MainWindow::on_task2_btn_clicked()
{
    ui->comboBox_program_type->setCurrentIndex(1);
    findParcely();
    type = ProgramType::Parcely;
}


void MainWindow::on_task3_btn_clicked()
{
    ui->comboBox_program_type->setCurrentIndex(2);

    findAreas();

    type = ProgramType::Area;

}

void MainWindow::on_task4_btn_clicked()
{
    addProperty();
    type = ProgramType::Nehnutelnosti;
    ui->comboBox_program_type->setCurrentIndex(0);
}

void MainWindow::on_task5_btn_clicked()
{
    addParcel();
    type = ProgramType::Parcely;
    ui->comboBox_program_type->setCurrentIndex(1);
}



void MainWindow::on_task6_btn_clicked()
{
    ProgramType originalType = type;

    editing_parameters();

    type = originalType;
}


void MainWindow::on_task8_btn_clicked()
{
    deleteSelectedItem(ui->list_of_items);
}


void MainWindow::on_refresh_btn_clicked()
{
    switch (type) {
    case ProgramType::Nehnutelnosti:
        RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
        break;
    case ProgramType::Parcely:
        RefreshListByParcel(ui->list_of_items, database.allParcely());
        break;
    case ProgramType::Area:
        RefreshListByArea(ui->list_of_items, database.allAreas());
        break;
    default:
        QMessageBox::warning(this, "Error", "Unknown Program Type!");
        break;
    }
}


