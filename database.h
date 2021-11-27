#pragma once

#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "gamewidget.h"

class DataBase
{
public:

    void connectToDatabase();

    void saveDataToDB(const QVector<DataTable> & dataVector, const QString &savingName, bool isAutoSave = false);

    const QVector<DataTable> & loadDataFromDB(const QString &savingName, const QString &savingDateTime);

    enum SAVING_COLUMN
    {
        NAME,
        DATE_TIME
    };

    QStringList getSavingColumnList(SAVING_COLUMN column) const;

    void setIdName(QSqlQuery &query, const QString & savingName, const QString &savingDateTime);

    void showTableOnDebugMode();

    QString getLastRecord(SAVING_COLUMN column) const;

    bool isFoundName(const QString & name);

    void deleteOldRecord(const QString & name);

    void disconnectDataBase();

    ~DataBase() { QSqlDatabase::database("DatabaseConnectionName").close(); }


private:

    QString tableName;

    QVector<DataTable> data;

    QString idName;
};
