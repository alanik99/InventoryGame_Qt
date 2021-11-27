#pragma once
#include <QDialog>
#include <QString>
#include <QTableWidget>

class ListDialog : public QDialog
{
    Q_OBJECT

public:
    ListDialog(QWidget * parent = nullptr);

signals:
    void getSavingName(const QString &, const QString &);

private slots:
    void onClicked();

private:
    QTableWidget * list;
};
