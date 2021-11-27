#include "database.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include <QDialog>
#include <QDateTime>
#include <QPair>

void DataBase::connectToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "DatabaseConnectionName");
    db.setHostName("localhost");
    db.setDatabaseName("dbName");

    if(!db.open())
    {
        qDebug() << "Failed to open DataBase";
        return;
    }

    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    if(!query.exec("CREATE TABLE IF NOT EXISTS first_table"
                   " ("
                   "idName INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "Name TEXT NOT NULL,"
                   "DateTime TEXT NOT NULL"
                   " )"))
        qDebug() << query.lastError().text();


    if(!query.exec("CREATE TABLE IF NOT EXISTS second_table"
                   " ("
                   "idName INTEGER NOT NULL, "
                   "id    INTEGER NOT NULL, "
                   "count INTEGER NOT NULL, "
                   "row   INTEGER NOT NULL, "
                   "col   INTEGER NOT NULL, "
                   "CONSTRAINT primary_key PRIMARY KEY (idName, row, col)"
                   " )"))
        qDebug() << query.lastError().text();
}

void DataBase::saveDataToDB(const QVector<DataTable> &dataVector, const QString &savingName, bool isAutoSave)
{
    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    QString currentDateTime;
    if ( isAutoSave )
        currentDateTime = "";
    else
        currentDateTime = QDateTime::currentDateTime().toString();


    if( !query.exec("INSERT INTO first_table (Name, DateTime) VALUES ('" + savingName + "', '" +
                    currentDateTime + "')" ))

        qDebug() << query.lastError().text();


    data = dataVector;

    setIdName(query, savingName, currentDateTime);

    for(int i = 0; i < data.size(); ++i)
    {
#ifdef DEBUG
        qDebug() << "save"
                 << data[i].id << "; "
                 << data[i].count << "; "
                 << data[i].row << "; "
                 << data[i].col << endl;
#endif

        query.prepare("INSERT INTO " + QString("second_table") +
                                               " (idName, "
                                                 "id, "
                                                 "count, "
                                                 "row, "
                                                 "col ) "
                      "VALUES (:IDNAME, :ID, :COUNT, :ROW, :COL)");

        query.bindValue(":IDNAME", idName);
        query.bindValue(":ID",    data[i].id);
        query.bindValue(":COUNT", data[i].count);
        query.bindValue(":ROW",   data[i].row);
        query.bindValue(":COL",   data[i].col);

        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }
    }
}

const QVector<DataTable> & DataBase::loadDataFromDB(const QString &savingName, const QString &savingDateTime)
{
    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    setIdName(query, savingName, savingDateTime);

    data.clear();

    if( !query.exec("SELECT * FROM second_table WHERE idName=" + idName) )
        qDebug() << query.lastError().text();

    while(query.next())
    {
        DataTable tmp;

        tmp.id    = query.value(1).toInt();
        tmp.count = query.value(2).toInt();
        tmp.row   = query.value(3).toInt();
        tmp.col   = query.value(4).toInt();

        data.push_back(tmp);
    }

    return data;
}

QStringList DataBase::getSavingColumnList(SAVING_COLUMN column) const
{
    QString col;
    if ( column == NAME )
        col = "Name";
    if ( column == DATE_TIME )
        col = "DateTime";


    QStringList result;

    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    if( !query.exec("SELECT " + col + " FROM first_table") )
        qDebug() << query.lastError().text();

    while(query.next())
        result << query.value(0).toString();

    return result;
}

void DataBase::setIdName(QSqlQuery &query, const QString &savingName, const QString &savingDateTime)
{
    if( !query.exec("SELECT idName FROM first_table WHERE Name='" + savingName + "'"
                    "AND DateTime = '" + savingDateTime + "'") )
        qDebug() << query.lastError().text();

    if( !query.first() )
        qDebug() << query.lastError().text();

    idName = query.value(0).toString();
}

void DataBase::disconnectDataBase()
{
    QSqlDatabase::removeDatabase( "DatabaseConnectionName" );
}

void DataBase::showTableOnDebugMode()
{
    QDialog tableWidget;

    QSqlTableModel model(&tableWidget, QSqlDatabase::database("DatabaseConnectionName") );
    model.setTable("second_table");
    if(!model.select())
        qDebug() << "model->select(): Error!";


    QTableView tableView;
    tableView.setSortingEnabled(true);
    tableView.setModel(&model);


    tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView.setSelectionMode(QAbstractItemView::SingleSelection);
    tableView.setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableView.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);


    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(&tableView);

    tableWidget.setLayout(layout);

    tableWidget.setMinimumSize(420, 320);

    tableWidget.show();

    tableWidget.exec();
}

QString DataBase::getLastRecord(SAVING_COLUMN column) const
{
    QString col;
    if ( column == NAME )
        col = "Name";
    if ( column == DATE_TIME )
        col = "DateTime";


    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    if( !query.exec("SELECT " + col + " FROM first_table WHERE idName IN ( SELECT max(idName) FROM first_table )") )
        qDebug() << query.lastError().text();

    if( query.first() )
    {
        return query.value(0).toString();
    }
    else
        return QString();

}

bool DataBase::isFoundName(const QString &name)
{
    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    if( !query.exec("SELECT Name FROM first_table") )
        qDebug() << query.lastError().text();

    while(query.next())
        if( query.value(0).toString() == name)
            return true;

    return false;
}

void DataBase::deleteOldRecord(const QString & name)
{
    QSqlQuery query(QSqlDatabase::database("DatabaseConnectionName"));

    setIdName(query, name, "");

    if( !query.exec("DELETE FROM  first_table WHERE idName='" + idName + "'") )
        qDebug() << query.lastError().text();
    if( !query.exec("DELETE FROM second_table WHERE idName='" + idName + "'") )
        qDebug() << query.lastError().text();
}
