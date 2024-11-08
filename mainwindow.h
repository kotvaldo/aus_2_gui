#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "TreeDatabase.h"

#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
enum class ProgramType {
    Nehnutelnosti,
    Parcely,
    Area
};
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void deleteSelectedItem(QListWidget* listWidget);
    void RefreshListByNehnutelnosti(QListWidget* listWidget, const std::vector<Nehnutelnost*>& nehnutelnosti);
    void RefreshListByArea(QListWidget* listWidget, const std::vector<Area*>& areas);
    void RefreshListByParcel(QListWidget* listWidget, const std::vector<Parcela*>& parcely);

private slots:






    void on_clear_btn_clicked();

    void on_save_button_clicked();

    void on_delete_btn_clicked();

    void on_load_from_file_clicked();

    void on_list_of_items_itemClicked(QListWidgetItem *item);

    void on_generate_btn_clicked();

    void on_comboBox_program_type_currentIndexChanged(int index);

    void on_task1_btn_clicked();


    void on_task2_btn_clicked();

    void on_task3_btn_clicked();

    void on_task4_btn_clicked();

    void on_task5_btn_clicked();

    void on_task6_btn_clicked();

    void on_task8_btn_clicked();

    void on_refresh_btn_clicked();

private:
    Ui::MainWindow *ui;
    TreeDatabase database;
    ProgramType type;
    void findProperties();
    void findAreas();
    void findParcely();
    void addProperty();
    void addParcel();
    void editing_parameters();
};
#endif // MAINWINDOW_H
