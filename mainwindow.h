#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>  // ISA QMainWindow
#include <QValidator> // USES QValidators to validate user inputs;
#include <QMessageBox> // USES QMessageBox to alert users of program events;
#include "gotcalc.h" // HASA GotCalc member for calulating G Over T;
#include "solarcalc.h" // HASA SolarCalc member for calculating Solar position;
#include "howto.h" // HASA HowTo to display the 'How To' Dialog;
#include "optionmenu.h" // HASA OptionMenu for providing user options;
#include "logfile.h" // HASA LogFile for logging calculations;

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
    void calculateSolarAzAlt(); // Calculates the Solar Azimuth and Altitude;
    void calculateGot(); // Calculates Gain Over Temperature;
    void setFrequencies(); // Used for interpolating data points;
    void save(); // Saves Log Files;
    void howTo(); // Opens a How To window;
    void options(); // Opens an Options Menu Window;

private:
    Ui::MainWindow *ui;
    GotCalc* mGotCalc; // Used for Calculating Gain Over Temperature;
    SolarCalc* mSolarCalc; // Used for Calculating Solar Position;
    HowTo* mHowTo; // 'How To' Dialog;
    OptionMenu* mOptions; // Options Menu Dialog;
    LogFile* mLogFile; // Used for Logging;

    QDoubleValidator mLatValid; // Validates the Latitude input;
    QDoubleValidator mLonValid; // Validates the Longitude input;
    QDoubleValidator mBeamwidthValid; // Validates the Beamwidth input;
    QDoubleValidator mMeasurementValid; // Validates hot and cold measurments;
    QDoubleValidator mFluxValid; // Validates Solar Flux input;

    QString mDefaultSaveLoc; // Holds the default save location;

    double mUpperFreq; // Upper frequency used for data interpolation;
    double mLowerFreq; // Lower frequency used for data interpolation;

    bool checkGotFields(void); // Ensures completion of G Over T fields;
    void loadSettings(void); // Loads settings;
};

#endif // MAINWINDOW_H
