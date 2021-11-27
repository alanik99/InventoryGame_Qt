#pragma once

#include <QWidget>
#include <QDrag>
#include <QMimeData>

namespace Ui {
class subject1;
}

class subject1 : public QWidget
{
    Q_OBJECT

public:

    explicit subject1(QWidget *parent = nullptr);

    ~subject1() override;

    void create();

    void setCounter(int value);

    int getCounter();

    void disableCounter();

    void setMimeData(const QString & data);

    void setRowCol(int, int);

    void setID(int ID);

    int getID();

    QDrag *makeDrag();


protected:

    virtual void mousePressEvent(QMouseEvent *event) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void paintEvent(QPaintEvent *event) override;

    virtual void mouseReleaseEvent(QMouseEvent *e) override;


private:

    Ui::subject1 *ui;

    QPoint m_dragStart;

    QPixmap m_pixmap;

    QString m_mimeData;

    int m_row;

    int m_col;

    int m_ID;
};
