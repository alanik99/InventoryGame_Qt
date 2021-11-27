#include "table.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QHeaderView>
#include "subject1.h"

table::table()
{
    setAcceptDrops(true);

    setDragDropMode(QAbstractItemView::InternalMove);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
      verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    resize(700, 700);

    setMinimumSize(200, 200);
}

const QVector<DataTable> table::getData() const
{
    QVector<DataTable> result;

    for(int i = 0; i < model()->rowCount(); ++i)
    {
        for(int j = 0; j < model()->columnCount(); ++j)
        {
            auto ptr = cellWidget(i, j);
            if(ptr != nullptr)
            {
                subject1 *subinTable = static_cast<subject1*>(ptr);

                DataTable data;

                data.row = i;
                data.col = j;
                data.id = subinTable->getID();
                data.count = subinTable->getCounter();

                result.push_back(data);
            }
        }
    }
    return result;
}

void table::setData(const QVector<DataTable> & data)
{
    for(int i = 0; i < model()->rowCount(); ++i)
    {
        for(int j = 0; j < model()->columnCount(); ++j)
        {
            auto ptr = cellWidget(i, j);

            if(ptr != nullptr)
                removeCellWidget(i, j);
        }
    }

    for(int i = 0; i < data.size(); ++i)
    {
        subject1 *subinTable = new subject1();

        subinTable->create();

        subinTable->setMimeData("aplication/subjectInTable");

        subinTable->setRowCol(data[i].row, data[i].col);

        subinTable->setID(data[i].id);

        subinTable->setCounter(data[i].count);

        setCellWidget(data[i].row, data[i].col, subinTable);
    }
}

void table::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "dragEnterEvent";

    if(event->mimeData()->hasFormat("aplication/subject") ||
       event->mimeData()->hasFormat("aplication/subjectInTable"))
    {
        event->acceptProposedAction();

        qDebug() << "event->acceptProposedAction";
    }
    else
    {
        event->ignore();

        qDebug() << "event->ignore()";
    }
}

void table::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent";

    QModelIndex indexCur = indexAt(event->pos());

    if(!indexCur.isValid())
        return;

    qDebug() << "row to = " << indexCur.row() << "; col to = " << indexCur.column();

    if(event->mimeData()->hasFormat("aplication/subject"))
    {
        handleApplicationSubject(event, indexCur.row(), indexCur.column());
    }
    if(event->mimeData()->hasFormat("aplication/subjectInTable"))
    {
            handleSubjectInTable(event, indexCur.row(), indexCur.column());
    }
}

void table::handleApplicationSubject(QDropEvent *event, int rowTo, int colTo)
{
    QByteArray data = event->mimeData()->data("aplication/subject");

    QDataStream in(&data, QIODevice::ReadOnly);

    int row;
    int col;
    int ID;

    in >> row >> col >> ID;

    qDebug() << "row from = " << row << "; col from = " << col << "; id = " << ID;

    auto ptr = cellWidget(rowTo, colTo);
    if(ptr == nullptr)
    {
        qDebug()<<"noWidget";

        subject1 *subinTable = new subject1();

        subinTable->create();

        subinTable->setMimeData("aplication/subjectInTable");

        subinTable->setRowCol(rowTo, colTo);

        subinTable->setID(ID);

        subinTable->setCounter(1);

        setCellWidget(rowTo, colTo, subinTable);
    }
    else
    {
        qDebug()<<"isWidget";

        subject1 *subinTable = static_cast<subject1*>(ptr);

        int counter = subinTable->getCounter();

        if(ID == subinTable->getID())
        {
            counter++;
            subinTable->setCounter(counter);
        }
    }
}

void table::handleSubjectInTable(QDropEvent *event, int rowTo, int colTo)
{
    QByteArray data = event->mimeData()->data("aplication/subjectInTable");

    QDataStream in(&data, QIODevice::ReadOnly);

    int rowFrom;
    int colFrom;
    int IDFrom;

    in >> rowFrom >> colFrom >> IDFrom;

    qDebug() << "row from = " << rowFrom << "; col from = " << colFrom << "; id = " << IDFrom;


    auto ptrFrom = cellWidget(rowFrom, colFrom);

    subject1 *subFrom = static_cast<subject1*>(ptrFrom);

    int counterFrom = subFrom->getCounter();

    qDebug() << "counterFrom = " << counterFrom;


    auto ptrTo = cellWidget(rowTo, colTo);
    if(ptrTo == nullptr)
    {
        qDebug()<<"noWidget";

        subject1 *subinTable = new subject1();

        subinTable->create();

        subinTable->setMimeData("aplication/subjectInTable");

        subinTable->setRowCol(rowTo, colTo);

        subinTable->setID(IDFrom);

        subinTable->setCounter(counterFrom);

        setCellWidget(rowTo, colTo, subinTable);

        removeCellWidget(rowFrom, colFrom);
    }
    else
    {
        qDebug()<<"isWidget";

        subject1 *subinTable = static_cast<subject1*>(ptrTo);

        int counterTo = subinTable->getCounter();

        int IDTo = subinTable->getID();

        if(IDFrom == IDTo)
        {
            qDebug() << "counterTo" << counterTo;

            subinTable->setCounter(counterFrom + counterTo);

            removeCellWidget(rowFrom, colFrom);
        }
    }
}
