#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include "solarcalc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void calculateSolarAzAlt();

private:
    Ui::MainWindow *ui;
    SolarCalc* mSolarCalc;

    QDoubleValidator mLatValid;
    QDoubleValidator mLonValid;
};

#endif // MAINWINDOW_H
