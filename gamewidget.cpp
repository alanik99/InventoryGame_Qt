#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QDebug>

gameWidget::gameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameWidget)
{
    ui->setupUi(this);
}

gameWidget::~gameWidget()
{
    delete ui;
}

void gameWidget::create()
{
    ui->HorizontalLayout->addWidget(&m_table);

    for(int i = 0; i < 3; ++i)
    {
        m_subjectArray[i].create();
        m_subjectArray[i].setID(i);
        m_subjectArray[i].disableCounter();
        m_subjectArray[i].setMimeData("aplication/subject");
        m_subjectArray[i].setRowCol(-1, -1);

        ui->VerticalLayout->addWidget(&m_subjectArray[i]);
    }

    loadButton.setText("Загрузить\nсохранение");
    ui->VerticalLayout->addWidget(&loadButton);

    connect(&loadButton,
            SIGNAL(clicked()),
            this,
            SIGNAL(loadClicked()));


    m_table.setRowCount(3);
    m_table.setColumnCount(3);

    for(int i = 0; i < 3; ++i)
    {
        m_table.setRowHeight(i, 100);
        m_table.setColumnWidth(i, 100);
    }
}

const QVector<DataTable> gameWidget::getDataTable() const
{
    return m_table.getData();
}

void gameWidget::setDataTable(const QVector<DataTable> & data)
{
    m_table.setData(data);
}




