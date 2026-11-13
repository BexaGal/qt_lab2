#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvtablemodel.h"
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_action_triggered();

    void on_MainTable_doubleClicked(const QModelIndex &index);

    void on_MainTable_clicked(const QModelIndex &index);

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_searchEdit_textChanged(const QString &arg1);
    void deleteRows();

private:
    Ui::MainWindow *ui;
    void updateTitle();
    CsvTableModel *m_model = nullptr;
    QSortFilterProxyModel *m_proxy = nullptr;
    QString m_path;
    bool m_modified = false;
};
#endif // MAINWINDOW_H
