#include "mainwindow.h"
#include <QMessageBox>
#include "./ui_mainwindow.h"

//zadaj dva subory na nacitanie
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , database("C:/Users/adams/Desktop/Nehnutelnosti.csv", "C:/Users/adams/Desktop/Parcely.csv")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1300, 780);

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

void MainWindow::deleteSelectedItem(QListWidget *listWidget)
{
    QListWidgetItem *selectedItem = listWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Warning", "No item selected!");
        return;
    }

    int uid = selectedItem->data(Qt::UserRole).toInt();

    int ret = QMessageBox::question(this,
                                    "Delete Confirmation",
                                    "Are you sure you want to delete this item?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        bool success = false;

        // Použite správne indexy pre width a length: UserRole + 6 a UserRole + 7
        QString widthText = selectedItem->data(Qt::UserRole + 6).toString();
        QString lengthText = selectedItem->data(Qt::UserRole + 7).toString();

        char width = !widthText.isEmpty() ? widthText.toStdString()[0] : 'N';
        char length = !lengthText.isEmpty() ? lengthText.toStdString()[0] : 'E';

        GPSParameters gpsParams{selectedItem->data(Qt::UserRole + 1).toDouble(),
                                selectedItem->data(Qt::UserRole + 2).toDouble(),
                                width,
                                length};

        if (type == ProgramType::Area) {
            bool isNehnutelnost = selectedItem->data(Qt::UserRole + 5).toBool();
            if (isNehnutelnost) {
                success = database.deleteNehnutelnostRecord(uid, gpsParams);
            } else {
                success = database.deleteParcelaRecord(uid, gpsParams);
            }

        } else if (type == ProgramType::Nehnutelnosti) {
            success = database.deleteNehnutelnostRecord(uid, gpsParams);
        } else if (type == ProgramType::Parcely) {
            success = database.deleteParcelaRecord(uid, gpsParams);
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

void MainWindow::RefreshListByNehnutelnosti(QListWidget *listWidget,
                                            const std::vector<Nehnutelnost *> &nehnutelnosti)
{
    listWidget->clear();
    for (Nehnutelnost *nehnutelnost : nehnutelnosti) {
        QString itemText
            = QString("Nehnutelnost UID: %1, {GPS: (%2, %3, %4, %5), Supisne Cislo: %6, Popis: %7}")
                  .arg(nehnutelnost->getUid())
                  .arg(nehnutelnost->getGps()->getX())
                  .arg(nehnutelnost->getGps()->getY())
                  .arg(nehnutelnost->getGps()->getWidth())
                  .arg(nehnutelnost->getGps()->getLength())
                  .arg(nehnutelnost->getSupisneCislo())
                  .arg(QString::fromStdString(nehnutelnost->getPopis()));

        QListWidgetItem *item = new QListWidgetItem(itemText, listWidget);

        item->setData(Qt::UserRole, nehnutelnost->getUid());
        item->setData(Qt::UserRole + 1, nehnutelnost->getGps()->getX());
        item->setData(Qt::UserRole + 2, nehnutelnost->getGps()->getY());
        item->setData(Qt::UserRole + 3, nehnutelnost->getSupisneCislo());
        item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->getPopis()));
        item->setData(Qt::UserRole + 6, QChar(nehnutelnost->getGps()->getWidth()));
        item->setData(Qt::UserRole + 7, QChar(nehnutelnost->getGps()->getLength()));
        item->setData(Qt::UserRole + 9, QVariant::fromValue(reinterpret_cast<void *>(nehnutelnost)));

        listWidget->addItem(item);
    }

    type = ProgramType::Nehnutelnosti;
    ui->label_count->setText("Počet nehnutelnosti: " + QString::number(nehnutelnosti.size()));
}

void MainWindow::RefreshListByArea(QListWidget *listWidget, const std::vector<Area *> &areas)
{
    listWidget->clear();
    for (Area *area : areas) {
        QString itemText;
        QListWidgetItem *item = nullptr;

        if (area->getNehnutelnost() != nullptr) {
            Nehnutelnost *nehnutelnost = area->getNehnutelnost();
            itemText = QString("Area ID: %1, {Nehnutelnost UID: %2, GPS: (%3, %4, %5, %6), Supisne "
                               "Cislo: %7, Popis: %8}")
                           .arg(area->getUid())
                           .arg(nehnutelnost->getUid())
                           .arg(nehnutelnost->getGps()->getX())
                           .arg(nehnutelnost->getGps()->getY())
                           .arg(nehnutelnost->getGps()->getWidth())
                           .arg(nehnutelnost->getGps()->getLength())
                           .arg(nehnutelnost->getSupisneCislo())
                           .arg(QString::fromStdString(nehnutelnost->getPopis()));

            item = new QListWidgetItem(itemText, listWidget);

            item->setData(Qt::UserRole, nehnutelnost->getUid());
            item->setData(Qt::UserRole + 1, nehnutelnost->getGps()->getX());
            item->setData(Qt::UserRole + 2, nehnutelnost->getGps()->getY());
            item->setData(Qt::UserRole + 3, nehnutelnost->getSupisneCislo());
            item->setData(Qt::UserRole + 4, QString::fromStdString(nehnutelnost->getPopis()));
            item->setData(Qt::UserRole + 5, true); // true znamená Nehnutelnost
            item->setData(Qt::UserRole + 6, QChar(nehnutelnost->getGps()->getWidth()));
            item->setData(Qt::UserRole + 7, QChar(nehnutelnost->getGps()->getLength()));
            item->setData(Qt::UserRole + 8, area->getUid());
            item->setData(Qt::UserRole + 9,
                          QVariant::fromValue(reinterpret_cast<void *>(nehnutelnost)));

        } else if (area->getParcela() != nullptr) {
            Parcela *parcela = area->getParcela();
            itemText = QString("Area ID: %1, {Parcela UID: %2, GPS: (%3, %4, %5, %6), Cislo "
                               "Parcely: %7, Popis: %8}")
                           .arg(area->getUid())
                           .arg(parcela->getUid())
                           .arg(parcela->getGps()->getX())
                           .arg(parcela->getGps()->getY())
                           .arg(parcela->getGps()->getWidth())
                           .arg(parcela->getGps()->getLength())
                           .arg(parcela->getCisloParcely())
                           .arg(QString::fromStdString(parcela->getPopis()));

            item = new QListWidgetItem(itemText, listWidget);

            item->setData(Qt::UserRole, parcela->getUid());
            item->setData(Qt::UserRole + 1, parcela->getGps()->getX());
            item->setData(Qt::UserRole + 2, parcela->getGps()->getY());
            item->setData(Qt::UserRole + 3, parcela->getCisloParcely());
            item->setData(Qt::UserRole + 4, QString::fromStdString(parcela->getPopis()));
            item->setData(Qt::UserRole + 5, false); // false znamená Parcela
            item->setData(Qt::UserRole + 6, QChar(parcela->getGps()->getWidth()));
            item->setData(Qt::UserRole + 7, QChar(parcela->getGps()->getLength()));
            item->setData(Qt::UserRole + 8, area->getUid());
            item->setData(Qt::UserRole + 9, QVariant::fromValue(reinterpret_cast<void *>(parcela)));
        }

        if (item != nullptr) {
            listWidget->addItem(item);
        }
    }

    type = ProgramType::Area;
    ui->label_count->setText("Počet area: " + QString::number(areas.size()));
}

void MainWindow::RefreshListByParcel(QListWidget *listWidget, const std::vector<Parcela *> &parcely)
{
    listWidget->clear();
    for (Parcela *parcela : parcely) {
        QString itemText
            = QString("Parcela UID: %1, {GPS: (%2, %3, %4, %5), Cislo Parcely: %6, Popis: %7}")
                  .arg(parcela->getUid())
                  .arg(parcela->getGps()->getX())
                  .arg(parcela->getGps()->getY())
                  .arg(parcela->getGps()->getWidth())
                  .arg(parcela->getGps()->getLength())
                  .arg(parcela->getCisloParcely())
                  .arg(QString::fromStdString(parcela->getPopis()));

        QListWidgetItem *item = new QListWidgetItem(itemText, listWidget);

        item->setData(Qt::UserRole, parcela->getUid());
        item->setData(Qt::UserRole + 1, parcela->getGps()->getX());
        item->setData(Qt::UserRole + 2, parcela->getGps()->getY());
        item->setData(Qt::UserRole + 3, parcela->getCisloParcely());
        item->setData(Qt::UserRole + 4, QString::fromStdString(parcela->getPopis()));
        item->setData(Qt::UserRole + 6, QChar(parcela->getGps()->getWidth()));
        item->setData(Qt::UserRole + 7, QChar(parcela->getGps()->getLength()));
        item->setData(Qt::UserRole + 8, parcela->getUid());
        item->setData(Qt::UserRole + 9, QVariant::fromValue(reinterpret_cast<void *>(parcela)));

        listWidget->addItem(item);
    }

    type = ProgramType::Parcely;
    ui->label_count->setText("Počet parciel: " + QString::number(parcely.size()));
}

void MainWindow::editing_parameters()
{
    this->ui->listWidget_prekryv->clear();
    QListWidgetItem *selectedItem = ui->list_of_items->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Warning", "No item selected!");
        return;
    }

    int uid = selectedItem->data(Qt::UserRole).toInt();

    GPSParameters oldCoords{selectedItem->data(Qt::UserRole + 1).toDouble(),
                            selectedItem->data(Qt::UserRole + 2).toDouble(),
                            selectedItem->data(Qt::UserRole + 6).toString().toStdString()[0],
                            selectedItem->data(Qt::UserRole + 7).toString().toStdString()[0]};

    GPSParameters newCoords{ui->x1_edit->text().toDouble(),
                            ui->y1_edit->text().toDouble(),
                            ui->comboBox_edit->currentText().toStdString()[0],
                            ui->comboBox_edit_2->currentText().toStdString()[0]};

    if (type == ProgramType::Area) {
        bool isNehnutelnost = selectedItem->data(Qt::UserRole + 5).toBool();
        if (isNehnutelnost) {
            NehnutelnostParameters newParams{ui->new_number->text().toInt(),
                                             ui->description->toPlainText().toStdString()};
            if (database.editNehnutelnost(uid, oldCoords, newCoords, newParams)) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
            }
        } else {
            ParcelaParameters newParams{ui->new_number->text().toInt(),
                                        ui->description->toPlainText().toStdString()};
            if (database.editParcela(uid, oldCoords, newCoords, newParams)) {
                RefreshListByArea(ui->list_of_items, database.allAreas());
            } else {
                QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
            }
        }
    } else if (type == ProgramType::Nehnutelnosti) {
        NehnutelnostParameters newParams{ui->new_number->text().toInt(),
                                         ui->description->toPlainText().toStdString()};
        if (database.editNehnutelnost(uid, oldCoords, newCoords, newParams)) {
            RefreshListByNehnutelnosti(ui->list_of_items, database.allNehnutelnosti());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Nehnutelnost!");
        }
    } else if (type == ProgramType::Parcely) {
        ParcelaParameters newParams{ui->new_number->text().toInt(),
                                    ui->description->toPlainText().toStdString()};
        if (database.editParcela(uid, oldCoords, newCoords, newParams)) {
            RefreshListByParcel(ui->list_of_items, database.allParcely());
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit Parcela!");
        }
    }
}

void MainWindow::on_clear_btn_clicked()
{
    this->ui->listWidget_prekryv->clear();
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
    this->ui->listWidget_prekryv->clear();
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
    this->ui->listWidget_prekryv->clear();
    database.LoadDataFromFile();
    RefreshListByArea(ui->list_of_items, database.allAreas());
    type = ProgramType::Area;
    ui->comboBox_program_type->setCurrentIndex(2);
}

void MainWindow::on_list_of_items_itemClicked(QListWidgetItem *item)
{
    ui->x1_edit->setText(QString::number(item->data(Qt::UserRole + 1).toDouble()));
    ui->y1_edit->setText(QString::number(item->data(Qt::UserRole + 2).toDouble()));
    ui->new_number->setText(item->data(Qt::UserRole + 3).toString());
    ui->description->setText(item->data(Qt::UserRole + 4).toString());
    ui->comboBox_edit->setCurrentText(item->data(Qt::UserRole + 6).toString());
    ui->comboBox_edit_2->setCurrentText(item->data(Qt::UserRole + 7).toString());

    ui->listWidget_prekryv->clear();

    if (type == ProgramType::Nehnutelnosti) {
        Nehnutelnost *nehnutelnost = reinterpret_cast<Nehnutelnost *>(
            item->data(Qt::UserRole + 9).value<void *>());
        if (nehnutelnost) {
            for (const auto &parcela : nehnutelnost->getParcely()) {
                QString itemText
                    = QString(
                          "Parcela UID: %1, GPS: (%2, %3, %4, %5), Cislo Parcely: %6, Popis: %7")
                          .arg(parcela->getUid())
                          .arg(parcela->getGps()->getX())
                          .arg(parcela->getGps()->getY())
                          .arg(parcela->getGps()->getWidth())
                          .arg(parcela->getGps()->getLength())
                          .arg(parcela->getCisloParcely())
                          .arg(QString::fromStdString(parcela->getPopis()));
                ui->listWidget_prekryv->addItem(itemText);
            }
        }
    } else if (type == ProgramType::Parcely) {
        Parcela *parcela = reinterpret_cast<Parcela *>(item->data(Qt::UserRole + 9).value<void *>());
        if (parcela) {
            for (const auto &nehnutelnost : parcela->getNehnutelnosti()) {
                QString itemText = QString("Nehnutelnost UID: %1, GPS: (%2, %3, %4, %5), Supisne "
                                           "Cislo: %6, Popis: %7")
                                       .arg(nehnutelnost->getUid())
                                       .arg(nehnutelnost->getGps()->getX())
                                       .arg(nehnutelnost->getGps()->getY())
                                       .arg(nehnutelnost->getGps()->getWidth())
                                       .arg(nehnutelnost->getGps()->getLength())
                                       .arg(nehnutelnost->getSupisneCislo())
                                       .arg(QString::fromStdString(nehnutelnost->getPopis()));
                ui->listWidget_prekryv->addItem(itemText);
            }
        }
    } else if (type == ProgramType::Area) {
        bool isNehnutelnost = item->data(Qt::UserRole + 5).toBool();
        if (isNehnutelnost) {
            Nehnutelnost *nehnutelnost = reinterpret_cast<Nehnutelnost *>(
                item->data(Qt::UserRole + 9).value<void *>());
            if (nehnutelnost) {
                for (const auto &parcela : nehnutelnost->getParcely()) {
                    QString itemText = QString("Parcela UID: %1, GPS: (%2, %3, %4, %5), Cislo "
                                               "Parcely: %6, Popis: %7")
                                           .arg(parcela->getUid())
                                           .arg(parcela->getGps()->getX())
                                           .arg(parcela->getGps()->getY())
                                           .arg(parcela->getGps()->getWidth())
                                           .arg(parcela->getGps()->getLength())
                                           .arg(parcela->getCisloParcely())
                                           .arg(QString::fromStdString(parcela->getPopis()));
                    ui->listWidget_prekryv->addItem(itemText);
                }
            }
        } else {
            Parcela *parcela = reinterpret_cast<Parcela *>(
                item->data(Qt::UserRole + 9).value<void *>());
            if (parcela) {
                for (const auto &nehnutelnost : parcela->getNehnutelnosti()) {
                    QString itemText = QString("Nehnutelnost UID: %1, GPS: (%2, %3, %4, %5), "
                                               "Supisne Cislo: %6, Popis: %7")
                                           .arg(nehnutelnost->getUid())
                                           .arg(nehnutelnost->getGps()->getX())
                                           .arg(nehnutelnost->getGps()->getY())
                                           .arg(nehnutelnost->getGps()->getWidth())
                                           .arg(nehnutelnost->getGps()->getLength())
                                           .arg(nehnutelnost->getSupisneCislo())
                                           .arg(QString::fromStdString(nehnutelnost->getPopis()));
                    ui->listWidget_prekryv->addItem(itemText);
                }
            }
        }
    }
}

void MainWindow::findProperties()
{
    GPSParameters coords1{ui->x1_edit->text().toDouble(),
                          ui->y1_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    bool hasSecondCoordinate = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    std::vector<Nehnutelnost *> results;

    if (hasSecondCoordinate) {
        GPSParameters coords2{ui->x2_edit->text().toDouble(),
                              ui->y2_edit->text().toDouble(),
                              ui->comboBox_edit->currentText().toStdString()[0],
                              ui->comboBox_edit_2->currentText().toStdString()[0]};

        BoundingBox bbox{coords1, coords2};
        results = database.findNehnutelnosti(bbox);
    } else {
        results = database.findNehnutelnostiOnly(coords1);
    }

    RefreshListByNehnutelnosti(ui->list_of_items, results);
    ui->label_count->setText("Počet nájdených nehnuteľností: " + QString::number(results.size()));
}

void MainWindow::findAreas()
{
    GPSParameters coords1{ui->x1_edit->text().toDouble(),
                          ui->y1_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    bool hasSecondCoordinate = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    std::vector<Area *> results;

    if (hasSecondCoordinate) {
        GPSParameters coords2{ui->x2_edit->text().toDouble(),
                              ui->y2_edit->text().toDouble(),
                              ui->comboBox_edit->currentText().toStdString()[0],
                              ui->comboBox_edit_2->currentText().toStdString()[0]};

        BoundingBox bbox{coords1, coords2};
        results = database.findAreas(bbox);
    } else {
        results = database.findAreasOnly(coords1);
    }

    RefreshListByArea(ui->list_of_items, results);
    ui->label_count->setText("Počet nájdených areas: " + QString::number(results.size()));
}

void MainWindow::findParcely()
{
    GPSParameters coords1{ui->x1_edit->text().toDouble(),
                          ui->y1_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    bool hasSecondCoordinate = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    std::vector<Parcela *> results;

    if (hasSecondCoordinate) {
        GPSParameters coords2{ui->x2_edit->text().toDouble(),
                              ui->y2_edit->text().toDouble(),
                              ui->comboBox_edit->currentText().toStdString()[0],
                              ui->comboBox_edit_2->currentText().toStdString()[0]};

        BoundingBox bbox{coords1, coords2};
        results = database.findParcely(bbox);
    } else {
        results = database.findParcelyOnly(coords1);
    }

    RefreshListByParcel(ui->list_of_items, results);
    ui->label_count->setText("Počet nájdených parciel: " + QString::number(results.size()));
}

#include <QMessageBox>

void MainWindow::addProperty()
{
    GPSParameters coords1{ui->x1_edit->text().toDouble(),
                          ui->y1_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    bool hasSecondCoordinates = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    NehnutelnostParameters params{ui->new_number->text().toInt(),
                                  ui->description->toPlainText().toStdString()};

    if (!hasSecondCoordinates) {
        QMessageBox::warning(this, "Nedostatok údajov", "Prosím, zadajte všetky súradnice.");
        return;
    }

    GPSParameters coords2{ui->x2_edit->text().toDouble(),
                          ui->y2_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    BoundingBox bbox{coords1, coords2};
    database.addNehnutelnost(bbox, params);
}

void MainWindow::addParcel()
{
    GPSParameters coords1{ui->x1_edit->text().toDouble(),
                          ui->y1_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    bool hasSecondCoordinates = !(ui->x2_edit->text().isEmpty() || ui->y2_edit->text().isEmpty());

    ParcelaParameters params{ui->new_number->text().toInt(),
                             ui->description->toPlainText().toStdString()};

    if (!hasSecondCoordinates) {
        QMessageBox::warning(this, "Nedostatok údajov", "Prosím, zadajte všetky súradnice.");
        return;
    }

    GPSParameters coords2{ui->x2_edit->text().toDouble(),
                          ui->y2_edit->text().toDouble(),
                          ui->comboBox_edit->currentText().toStdString()[0],
                          ui->comboBox_edit_2->currentText().toStdString()[0]};

    BoundingBox bbox{coords1, coords2};
    database.addParcela(bbox, params);
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
    ui->listWidget_prekryv->clear();
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
