#pragma once

#include <QWidget>
#include <QPushButton>
#include "subject1.h"
#include "table.h"

namespace Ui {
class gameWidget;
}

class gameWidget : public QWidget
{
    Q_OBJECT

public:

    explicit gameWidget(QWidget *parent = nullptr);

    ~gameWidget();

    void create();

    const QVector<DataTable> getDataTable() const;

    void setDataTable(const QVector<DataTable> &data);


signals:

    void loadClicked();


private:

    Ui::gameWidget *ui;

    subject1 m_subjectArray[3];

    table m_table;

    QPushButton loadButton;
};
