#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include <QMessageBox>
#include "gotcalc.h"
#include "solarcalc.h"
#include "howto.h"
#include "optionmenu.h"
#include "logfile.h"

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
    void save();
    void howTo();
    void options();

private:
    Ui::MainWindow *ui;
    GotCalc* mGotCalc;
    SolarCalc* mSolarCalc;
    HowTo* mHowTo;
    OptionMenu* mOptions;
    LogFile* mLogFile;

    QDoubleValidator mLatValid;
    QDoubleValidator mLonValid;
    QDoubleValidator mBeamwidthValid;
    QDoubleValidator mMeasurementValid;
    QDoubleValidator mFluxValid;

    QString mDefaultSaveLoc;

    double mUpperFreq;
    double mLowerFreq;

    bool checkGotFields(void);
    void loadSettings(void);
};

#endif // MAINWINDOW_H
