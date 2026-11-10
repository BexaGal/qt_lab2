#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csvtablemodel.h"
#include "errorwindow.h"
#include "editdialog.h"

CsvTableModel FileHandler;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}
// START CODE?


void MainWindow::on_action_triggered()
{
    if(!FileHandler.loadFromFile("C:/Temp/File")){
        ErrorWindow FileError;
        if(FileError.exec()){

        }
    }
    ui->MainTable->setModel(&FileHandler);
}



void MainWindow::on_MainTable_doubleClicked(const QModelIndex &index)
{
    EditDialog editDialog;
    if(editDialog.exec()){

    }
}


void MainWindow::on_MainTable_clicked(const QModelIndex &index)
{
    EditDialog editDialog;
    editDialog.TransferData(FileHandler.data(index));
    if(editDialog.exec()){
        FileHandler.setData(index, editDialog.containedData);
    }

}


void MainWindow::on_action_2_triggered()
{
    if(!FileHandler.saveToFile("C:/Temp/File")){
        ErrorWindow FileError;
        if(FileError.exec()){

        }
    }
}

