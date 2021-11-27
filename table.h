#pragma once

#include <QTableWidget>

struct DataTable
{
    int id;
    int count;
    int row;
    int col;

    bool operator==(const DataTable & other) const
    {
        return    this->id == other.id    &&
               this->count == other.count &&
                 this->row == other.row   &&
                 this->col == other.col;
    }
};

class table : public QTableWidget
{
public:

    table();

    const QVector<DataTable> getData() const;

    void setData(const QVector<DataTable> & data);


protected:

    virtual void dragEnterEvent(QDragEnterEvent* event) override;

    virtual void dropEvent(QDropEvent *event) override;


private:

    void handleApplicationSubject(QDropEvent *event, int rowTo, int colTo);

    void handleSubjectInTable(QDropEvent *event, int rowTo, int colTo);
};
