#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include <QMessageBox>
#include "gotcalc.h"
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
    void calculateGot();
    void setFrequencies();
    void howTo();

private:
    Ui::MainWindow *ui;
    GotCalc* mGotCalc;
    SolarCalc* mSolarCalc;
    HowTo* mHowTo;

    QDoubleValidator mLatValid;
    QDoubleValidator mLonValid;
    QDoubleValidator mBeamwidthValid;
    QDoubleValidator mMeasurementValid;
    QDoubleValidator mFluxValid;

    void checkGotFields(void);
};

#endif // MAINWINDOW_H
