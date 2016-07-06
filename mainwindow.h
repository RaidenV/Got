#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include "solarcalc.h"
#include "howto.h"

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
    void howTo();

private:
    Ui::MainWindow *ui;
    SolarCalc* mSolarCalc;
    HowTo* mHowTo;

    QDoubleValidator mLatValid;
    QDoubleValidator mLonValid;
};

#endif // MAINWINDOW_H
