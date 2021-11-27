#include "listdialog.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <database.h>
#include <QHeaderView>

ListDialog::ListDialog(QWidget * parent) : QDialog(parent)
{
    setWindowTitle("Choose saving");

    setMinimumSize(450, 300);

    QVBoxLayout * vlayout = new QVBoxLayout;

    list = new QTableWidget;
    list->setSelectionBehavior(QAbstractItemView::SelectRows);
    list->setColumnCount(2);

    list->setHorizontalHeaderLabels( QStringList() << "Name:" << "DateTime:" );
    list->verticalHeader()->hide();

    list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    list->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    list->resizeColumnsToContents();

    DataBase db;
    db.connectToDatabase();
    QStringList     names = db.getSavingColumnList( DataBase::NAME );
    QStringList dateTimes = db.getSavingColumnList( DataBase::DATE_TIME );
    db.disconnectDataBase();

    for (int i = 0; i < names.size(); ++i)
    {
        QTableWidgetItem *     itemName = new QTableWidgetItem( names[i] );
        QTableWidgetItem * itemDateTime = new QTableWidgetItem( dateTimes[i] );

        list->insertRow(i);
        list->setItem(i, 0, itemName);
        list->setItem(i, 1, itemDateTime);
    }


    QPushButton * okBtn = new QPushButton("ok");


    vlayout->addWidget(list);
    vlayout->addWidget(okBtn);
    setLayout(vlayout);

    connect(okBtn, SIGNAL(clicked()), this, SLOT(onClicked()));
    connect(okBtn, SIGNAL(clicked()), this, SLOT(close()));
}

void ListDialog::onClicked()
{
    auto curRow = list->currentRow();

    if(curRow != -1)
        emit getSavingName( list->item(curRow, 0)->text(),
                            list->item(curRow, 1)->text() );
}
