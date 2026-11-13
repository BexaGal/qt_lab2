#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csvtablemodel.h"
#include "errorwindow.h"
#include "editdialog.h"
#include "addrowdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>    // std::sort
#include <functional>   // std::greater


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_model = new CsvTableModel(this);
    m_proxy = new QSortFilterProxyModel(this);
    m_proxy->setSourceModel(m_model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxy->setFilterKeyColumn(-1);

    ui->MainTable->setModel(m_proxy);

    ui->searchEdit->setPlaceholderText(tr("Search..."));
    ui->searchEdit->setClearButtonEnabled(true);

    updateTitle();
}

void MainWindow::deleteRows()
{
    // 1) The rows that the user selected. These are VIEW indexes.
    const QModelIndexList selected = ui->MainTable->selectionModel()->selectedRows();

    // 2) Convert each one to a MODEL row number and keep the numbers in a list.
    QList<int> rows;
    for (const QModelIndex &proxyIndex : selected)
        rows << m_proxy->mapToSource(proxyIndex).row();

    // 3) Sort from the biggest number to the smallest.
    std::sort(rows.begin(), rows.end(), std::greater<int>());

    // 4) Delete the rows one by one, using MODEL row numbers.
    for (int row : std::as_const(rows))
        m_model->removeRows(row, 1);
}

void MainWindow::updateTitle()
{
    const QString name = m_path.isEmpty() ? tr("Untitled") : m_path;
    setWindowTitle(name + (m_modified ? " *" : ""));
}

MainWindow::~MainWindow()
{
    delete ui;

}
// START CODE?


void MainWindow::on_action_triggered()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open CSV"), {}, tr("CSV files (*.csv);;All files (*)"));
    if (path.isEmpty())
        return;                                   // user pressed Cancel

    if (!m_model->loadFromFile(path)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read the file."));
        return;
    }

    m_path = path;                                // remember the file for Save
    m_modified = false;
    updateTitle();

    if(!m_model->loadFromFile(m_path)){
        ErrorWindow FileError;
        if(FileError.exec()){

        }
    }
}



void MainWindow::on_MainTable_doubleClicked(const QModelIndex &index)
{
    EditDialog editDialog;
    if(editDialog.exec()){

    }
}


void MainWindow::on_MainTable_clicked(const QModelIndex &index)
{
    const QModelIndex src = m_proxy->mapToSource(index);
    EditDialog editDialog;
    editDialog.TransferData(m_model->data(src));
    if(editDialog.exec()){
        m_model->setData(src, editDialog.containedData);
    }

}


void MainWindow::on_action_2_triggered()
{
    if(!m_model->saveToFile(m_path)){
        ErrorWindow FileError;
        FileError.errorMessage = "Failed to save file.";
        if(FileError.exec()){

        }
    }
}


void MainWindow::on_action_3_triggered()
{
    AddRowDialog RowsInserter;
    if (RowsInserter.exec()){
        m_model->insertRows(RowsInserter.fromRow, RowsInserter.rows);
    }
}


void MainWindow::on_action_4_triggered()
{
    // 1) Ask the user where to create the file
    QString path = QFileDialog::getSaveFileName(
        this, tr("New CSV"), {}, tr("CSV files (*.csv)"));
    if (path.isEmpty())
        return;                                    // user pressed Cancel

    // 2) The dialog may not add the extension
    if (!path.endsWith(".csv", Qt::CaseInsensitive))
        path += ".csv";

    // 3) Create the file with the header line
    if (!m_model->createFile(path, {"Name", "Age", "City"})) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot create the file."));
        return;
    }

    // 4) Remember the file, so "Save" writes to it
    m_path = path;
    m_modified = false;
    updateTitle();
}


void MainWindow::on_searchEdit_textChanged(const QString &arg1)
{
    m_proxy->setFilterFixedString(arg1);
}

