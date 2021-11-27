#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QInputDialog>
#include <listdialog.h>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPair>

#define DEBUG      // shows SQL-Table
#undef DEBUG     // doesn't show SQL-Table

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->saveAction,
            SIGNAL(triggered()),
            this,
            SLOT(saveClick()));

    connect(ui->loadAction,
            SIGNAL(triggered()),
            this,
            SLOT(loadClick()));

    connect(&my_widget,
            SIGNAL(loadClicked()),
            this,
            SLOT(loadClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create()
{
   my_widget.create();

   setCentralWidget(&my_widget);

   // set last game on table
   db.connectToDatabase();

   loadName     = db.getLastRecord( DataBase::NAME );
   loadDateTime = db.getLastRecord( DataBase::DATE_TIME );

   if ( loadName != QString() )
       my_widget.setDataTable(   db.loadDataFromDB( loadName, loadDateTime )   );

#ifdef DEBUG
   db.showTableOnDebugMode();
#endif
   db.disconnectDataBase();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QString autoSavingName = "autosave";
    QString autoSavingDateTime = "";

    db.connectToDatabase();

    if( my_widget.getDataTable() == db.loadDataFromDB(autoSavingName, autoSavingDateTime) )
        return;  // nothing needs to be updated

    setWindowTitle("Saving... Wait a while");

    if( db.isFoundName(autoSavingName) )
    {
        db.deleteOldRecord(autoSavingName);
    }

    db.saveDataToDB( my_widget.getDataTable(), autoSavingName, true);

#ifdef DEBUG
    db.showTableOnDebugMode();
#endif
    db.disconnectDataBase();
}

void MainWindow::saveClick()
{
    bool isAccepted;
    QString savingName = QInputDialog::getText(this, tr("Input saving name"),
                                         tr("Input saving name:"), QLineEdit::Normal, "", &isAccepted);
    if (isAccepted)
    {
        if(!savingName.isEmpty())
        {
            db.connectToDatabase();

    //        if( db.isFoundName(savingName) )
    //        {
    //            QMessageBox::StandardButton reply;

    //            reply = QMessageBox::question(this, "Overwriting",
    //                                 "Saving name \"" + savingName + "\" is already in the database, overwrite?",
    //                                 QMessageBox::Yes|QMessageBox::No);

    //            if (reply == QMessageBox::Yes)
    //                db.deleteOldRecord(savingName);
    //        }

            db.saveDataToDB( my_widget.getDataTable(), savingName);

            statusBar()->setFont( QFont("Sans Serif", 6) );
            statusBar()->showMessage( "Saved to table: " + savingName, 3000 );

    #ifdef DEBUG
            db.showTableOnDebugMode();
    #endif
            db.disconnectDataBase();
        }
        else
        {
            QMessageBox::warning(this, "Empty name", "Name shouldn't be empty");
            saveClick();
        }
    }
}


void MainWindow::loadClick()
{
    loadName     = "";
    loadDateTime = "";

    ListDialog listDialog;

    connect( &listDialog,
             SIGNAL(getSavingName(QString, QString)),
               this,
               SLOT(setSavingName(QString, QString))  );

    listDialog.exec();

    if(!loadName.isEmpty())
    {
        db.connectToDatabase();

        my_widget.setDataTable( db.loadDataFromDB(loadName, loadDateTime) );

#ifdef DEBUG
        db.showTableOnDebugMode();
#endif
        db.disconnectDataBase();
    }
}

void MainWindow::setSavingName(const QString &name, const QString &dateTime)
{
    loadName      = name;
    loadDateTime  = dateTime;
}

