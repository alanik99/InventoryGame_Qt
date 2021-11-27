#include "subject1.h"
#include "ui_subject1.h"
#include <QPixmap>
#include <QVariant>
#include <QtDebug>
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>

subject1::subject1(QWidget *parent) : QWidget(parent),
    ui(new Ui::subject1)
{
    ui->setupUi(this);
}

subject1::~subject1()
{
    delete ui;
}

void subject1::create()
{
    setCounter(0);
}

void subject1::setCounter(int value)
{
    QVariant var(value);
    ui->counter->setText(var.toString());
}

int subject1::getCounter()
{
    QVariant var(ui->counter->text());
    return var.toInt();
}

void subject1::disableCounter()
{
    ui->counter->setVisible(false);
}

void subject1::setMimeData(const QString &data)
{
    m_mimeData = data;
}

void subject1::setRowCol(int row, int col)
{
    m_row = row;
    m_col = col;
}

void subject1::setID(int ID)
{
    m_ID = ID;

    if(m_ID == 0)
    {
        m_pixmap.load(":/images/basket.webp");
    }
    else if(m_ID == 1)
    {
        m_pixmap.load(":/images/foot.webp");
    }
    else if(m_ID == 2)
    {
        m_pixmap.load(":/images/rugby.jpg");
    }
    else
    {
        m_pixmap.load("");
    }
}

int subject1::getID()
{
    return m_ID;
}

void subject1::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent";

    m_dragStart = event->pos();

    QWidget::mousePressEvent(event);
}

void subject1::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() <<"mouseMoveEvent";

    if(    (event->buttons() & Qt::LeftButton)
           && QApplication::startDragDistance() <= (event->pos() - m_dragStart).manhattanLength()    )
    {
        qDebug() << "QApplication::startDragDistance() <= (event->pos() - m_dragStart).manhattanLength()";
        makeDrag()->exec(Qt::MoveAction);
    }

    QWidget::mouseMoveEvent(event);
}

void subject1::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (!m_pixmap.isNull())
    {
        QSize widgetSize = size();

        QPixmap scaledPixmap = m_pixmap.scaled(widgetSize, Qt::KeepAspectRatio);

        QPoint center( (widgetSize.width()-scaledPixmap.width())/2,
                      (widgetSize.height()-scaledPixmap.height())/2);

        painter.drawPixmap(center, scaledPixmap);
    }

    QWidget::paintEvent(event);
}

QDrag *subject1::makeDrag()
{
    QDrag *drag = new QDrag(this);


    QByteArray itemData;

    QDataStream stream(&itemData, QIODevice::WriteOnly);

    stream << m_row << m_col << m_ID;


    QMimeData *mimeData = new QMimeData;

    mimeData->setData(m_mimeData, itemData);


    drag->setMimeData(mimeData);

    drag->setPixmap(m_pixmap.scaled(size(), Qt::KeepAspectRatio));

    return drag;
}

void subject1::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "mouseReleaseEvent";

    QWidget:: mouseReleaseEvent(e);
}

