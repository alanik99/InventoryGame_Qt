#pragma once

#include <QMainWindow>
#include "gamewidget.h"
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void create();


protected:

    void closeEvent(QCloseEvent *) override;


private:

    Ui::MainWindow *ui;

    gameWidget my_widget;

    DataBase db;

    QString loadName;

    QString loadDateTime;


private slots:

    void saveClick();

    void loadClick();

    void setSavingName(const QString & name, const QString & dateTime);
};
