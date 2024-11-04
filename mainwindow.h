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

    void on_load_data_btn_clicked();

    void on_add_parcel_btn_clicked();

    void on_show_propert_btn_clicked();

    void on_show_parcels_btn_clicked();

    void on_show_areas_btn_clicked();

    void on_edit_selected_btn_clicked();

    void on_add_property_btn_clicked();

    void on_refresh_btn_clicked();

    void on_clear_btn_clicked();

    void on_save_button_clicked();

    void on_delete_btn_clicked();

    void on_show_duplicities_btn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    TreeDatabase database;
    ProgramType type;
};
#endif // MAINWINDOW_H
