/*-----------------------------------------------------------------------------
Name         mainwindow.cpp

Purpose      Handles the majority of user input;

History		 11 Jun 16  AFB	Created
-----------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*----------------------------------------------------------------------------
Name         MainWindow

Purpose      Constructor;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Gain Over Temperature Calculator");

    // Initialize pointer variables;
    mSolarCalc = new SolarCalc(this);
    mGotCalc = new GotCalc(this);

    // Initialize member variables to zero;
    mDefaultSaveLoc = "";
    mUpperFreq = 0;
    mLowerFreq = 0;

    // Set the UI Time/Date edits to the current time found on the system;
    ui->timeEdit->setTime(QTime::currentTime());
    ui->dateEdit->setDate(QDate::currentDate());

    // Set up the various validators for the user inputs;
    mLatValid.setRange(-90, 90, 6);
    mLatValid.setNotation(QDoubleValidator::StandardNotation);

    mLonValid.setRange(-180, 180, 6);
    mLonValid.setNotation(QDoubleValidator::StandardNotation);

    mBeamwidthValid.setRange(0, 100, 2);
    mBeamwidthValid.setNotation(QDoubleValidator::StandardNotation);

    mMeasurementValid.setRange(-200, 100, 2);
    mMeasurementValid.setNotation(QDoubleValidator::StandardNotation);

    mFluxValid.setRange(0, 1000, 2);
    mFluxValid.setNotation(QDoubleValidator::StandardNotation);

    // Assign the validators to the various user input fields;
    ui->lineEditLatitude->setValidator(&mLatValid);
    ui->lineEditLongitude->setValidator(&mLonValid);
    ui->lineEditBeamWidth->setValidator(&mBeamwidthValid);
    ui->lineEditHot1->setValidator(&mMeasurementValid);
    ui->lineEditHot2->setValidator(&mMeasurementValid);
    ui->lineEditHot3->setValidator(&mMeasurementValid);
    ui->lineEditCold1->setValidator(&mMeasurementValid);
    ui->lineEditCold2->setValidator(&mMeasurementValid);
    ui->lineEditCold3->setValidator(&mMeasurementValid);
    ui->lineEditSolarFluxHigh->setValidator(&mFluxValid);
    ui->lineEditSolarFluxLow->setValidator(&mFluxValid);
    ui->lineEditAntennaFrequency->setValidator(
                new QIntValidator(0, 50000, this));

    // Connect the necessary signals and slots;
    connect(ui->pushButtonCalculateSolarAzAlt, SIGNAL(clicked(bool))
            , this, SLOT(calculateSolarAzAlt()));
    connect(ui->pushButtonCalculateGot, SIGNAL(clicked())
            , this, SLOT(calculateGot()));
    connect(ui->pushButtonSave, SIGNAL(clicked())
            , this, SLOT(save()));
    connect(ui->lineEditAntennaFrequency, SIGNAL(editingFinished())
            , this, SLOT(setFrequencies()), Qt::UniqueConnection);
    connect(ui->actionHowTo, SIGNAL(triggered()), this, SLOT(howTo()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(options()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    // Load the settings, in this case the default save directory;
    loadSettings();

}

/*----------------------------------------------------------------------------
Name         ~MainWindow

Purpose      Destructor;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    // All objects on the heap are QObject type and will be handled
    // automatically;
    delete ui;
}

/*----------------------------------------------------------------------------
Name         calculateSolarAzAlt

Purpose      Calculates the values of the Solar Altitude and Elevation which
             the antenna should be pointed to in order to face the sun
             directly;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::calculateSolarAzAlt()
{
    // Set the longitude and latitude of the SolarCalc object to that which
    // was entered by the user;
    mSolarCalc->setLatitude(ui->lineEditLatitude->text().toDouble());
    mSolarCalc->setLongitude(ui->lineEditLongitude->text().toDouble());

    // QTime and QDate objects to pass to the mSolarCalc object;
    QTime rTime(ui->timeEdit->time());
    QDate rDate(ui->dateEdit->date());

    // Set the Time, Date, and whether or not it is currently Daylight Savings
    // Time, in the mSolarCalc object;
    mSolarCalc->setTime(rTime);
    mSolarCalc->setDate(rDate);
    mSolarCalc->setDaylightSavings(ui->checkBoxIsDaylightSavings->isChecked());

    // Issue the calculate command.  This calculates the azimuth and altitude
    // of the Sun;
    mSolarCalc->calculate();

    // Get the resulting Azimuth and Altitude from the mSolarCalc object;
    ui->lineEditSolarAltitude->setText(
                QString::number(mSolarCalc->getSolarAltitude()));
    ui->lineEditSolarAzimuth->setText(
                QString::number(mSolarCalc->getSolarAzimuth()));
}

/*----------------------------------------------------------------------------
Name         calculateGot

Purpose      Calculates the Gain Over Temperature;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::calculateGot()
{
    //  Check to make sure that all the necessary fields have been completed;
    if (!checkGotFields())
    {
        // if not, return;
        return;
    }

    // Grab the upper and lower frequencies from the mGotCalc object's list
    // of available frequencies.  The idea behind this is that, if we are to
    // exponentially interpolate between two points, we need to (x,y)
    // coordinates.  The upper and lower frequencies give us our x coordinates
    // and the Solar Flux values entered by the user give us our y coordinates;
    setFrequencies();

    // Clear the hot and cold measurement vectors;
    mGotCalc->clearHotMeasurments();
    mGotCalc->clearColdMeasurments();

    // Add all of the user-entered measurements into the approprate vectors;
    mGotCalc->addHotMeasurement(ui->lineEditHot1->text().toDouble());
    mGotCalc->addHotMeasurement(ui->lineEditHot2->text().toDouble());
    mGotCalc->addHotMeasurement(ui->lineEditHot3->text().toDouble());

    mGotCalc->addColdMeasurement(ui->lineEditCold2->text().toDouble());
    mGotCalc->addColdMeasurement(ui->lineEditCold2->text().toDouble());
    mGotCalc->addColdMeasurement(ui->lineEditCold2->text().toDouble());

    // Set our y values for exponential interpolation;
    mGotCalc->setSolarFluxHigh(ui->lineEditSolarFluxHigh->text().toDouble());
    mGotCalc->setSolarFluxLow(ui->lineEditSolarFluxLow->text().toDouble());

    // Set the antenna beamwidth;
    mGotCalc->setBeamwidth(ui->lineEditBeamWidth->text().toDouble());

    // Call the calculate, which will produce our Gain Over Temperature value;
    mGotCalc->calculate();

    // Display that value to the user;
    ui->lineEditGotOutput->setText(QString::number(mGotCalc->getGotRatiodB()));
}

/*----------------------------------------------------------------------------
Name         setFrequencies

Purpose      Evaluates the operational frequency entered by the user against
             frequencies for which the user is able to obtain values of solar
             flux.

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::setFrequencies()
{
    std::vector<double> frequencyList;
    // Get the frequencies for which we're able to obtain solar flux values;
    mGotCalc->getAvailableFrequencies(frequencyList);

    // Temporary variables for holding the frequency data;
    double lowerFrequency = 0;
    double higherFrequency = 0;

    // The frequency which was entered by the user;
    double targetFrequency = ui->lineEditAntennaFrequency->text().toDouble();

    // If the frequency entered by the user is less than the minimum frequency
    // for which we're able to obtain solar flux data, or greater than the
    // largest frequency for which we're able to obtain solar flux data;
    if (targetFrequency < frequencyList.at(0)
            || targetFrequency > frequencyList.at(frequencyList.size() - 1))
    {
        // Explain that the user's value is out of bounds, giving them the
        // boundaries within which they can entered a frequency;
        QMessageBox::critical(this, "Critical"
              , ("Please enter a frequency between "
               + QString::number(frequencyList.at(0))
               + " and "
               + QString::number(frequencyList.at(frequencyList.size() - 1))));
        // Set the frequency to the first frequency available in teh vector;
        ui->lineEditAntennaFrequency->setText(
                    QString::number(frequencyList.at(0)));
        // Run this again so that the Solar Flux @ XXXX MHz updates;
        setFrequencies();

        return;
    }

    // Loop through the available frequencies;
    for (size_t i = 0; i < frequencyList.size(); i++)
    {
        // If the frequency entered by the user is less than a given frequency
        // and the higher frequency has yet to be changed (still equals zero)
        // then that frequency equals the upper bounds;
        if (targetFrequency < frequencyList.at(i) && higherFrequency == 0)
        {
            // Set the higher frequency;
            higherFrequency = frequencyList.at(i);
            // If the higher frequency has been set, then the lower frequency
            // is obviously the frequency before it (given an ordered list);
            if (higherFrequency != 0)
            {
                // Keep in mind that the bounds error has already been prevented
                // two 'if' statements ago;
                lowerFrequency = frequencyList.at(i - 1);
            }
        }
    }

    // Set the Labels to let the user know which Solar Flux values they should
    // inquire about;
    ui->labelSolarFluxUpper->setText(
                "Solar Flux @ " + QString::number(higherFrequency) + " MHz");
    ui->labelSolarFluxLower->setText(
                "Solar Flux @ " + QString::number(lowerFrequency) + " MHz");

    // Set the frequencies within the Gain Over Temperature calculator;
    mGotCalc->setOperatingFrequency(targetFrequency);
    mGotCalc->setHigherFrequency(higherFrequency);
    mGotCalc->setLowerFrequency(lowerFrequency);

    // Set the member variables for future use;
    mUpperFreq = higherFrequency;
    mLowerFreq = lowerFrequency;
}

/*----------------------------------------------------------------------------
Name         save

Purpose      Produces an output file of the data gathered through the Gain Over
             Temperature and Solar Azimuth/Altitude calculations;

History		 11 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::save()
{
    // Access the global settings;
    QCoreApplication::setOrganizationName("CPI");
    QCoreApplication::setApplicationName("Got");
    QSettings settings;

    // File dialog for getting the location/name of the save file;
    QFileDialog fileDialog;

    QString filename;

    // Get the date and time in order to automatically generate a filename;
    QDateTime dT = QDateTime::currentDateTime();

    // Set the default suffix to .txt, meaning we're creating a text file;
    fileDialog.setDefaultSuffix(tr(".txt"));

    // If a default save location hasn't been selected, go to the standard
    // "My Documents" location, cross-platform;
    if(!settings.contains("DefaultSaveLoc"))
    {
        filename = fileDialog.getSaveFileName(this
           , tr("Save File")
           , QStandardPaths::displayName(QStandardPaths::DocumentsLocation)
             + "/"
             + dT.toString("dd-MMM-yyyy.HH.mm.ss")
           , "Text File (*.txt)");
    }

    // Otherwise, go to the default save location;
    else
    {
        filename = fileDialog.getSaveFileName(this
           , tr("Save File")
           , settings.value("DefaultSaveLoc").toString()
             + "/"
             + dT.toString("ddd-MMM-yyyy.HH.mm.ss")
           , "Text File (*.txt)");
    }

    // If the user hasn't entered a filename, return;
    if(filename.isEmpty())
    {
        return;
    }

    // Create a new LogFile object;
    mLogFile = new LogFile(this);

    // Open the Log File using the user-provided filename;
    mLogFile->setNameAndOpen(filename);

    // If the Solar Azimuth/Altitude was calculated, add it to the log file;
    if (!ui->lineEditSolarAltitude->text().isEmpty())
    {
        mLogFile->append(
                    tr("Solar Data Was Calculated: "));
        mLogFile->append(
                    "Date: "
                    + ui->dateEdit->date().toString());
        mLogFile->append(
                    "Time: "
                    + ui->timeEdit->time().toString());
        mLogFile->append(
                    "Latitude: "
                    + ui->lineEditLatitude->text());
        mLogFile->append(
                    "Longitude: "
                    + ui->lineEditLongitude->text());
        mLogFile->append(
                    "Solar Azimuth: "
                    + ui->lineEditSolarAzimuth->text());
        mLogFile->append(
                    "Solar Altitude: "
                    + ui->lineEditSolarAltitude->text());
    }

    // If the Gain Over Temperature was calculated, add it to the log file;
    if (!ui->lineEditGotOutput->text().isEmpty())
    {
        mLogFile->append(
                    tr("\rGain Over Temperature Data: "));
        mLogFile->append(
                    "Operating Frequency (MHz): "
                    + ui->lineEditAntennaFrequency->text());
        mLogFile->append(
                    "Beamwidth (Degrees): "
                    + ui->lineEditBeamWidth->text());
        mLogFile->append(
                    "Solar Flux @ "
                    + QString::number(mLowerFreq)
                    + " (MHz): "
                    + ui->lineEditSolarFluxLow->text());
        mLogFile->append(
                    "Solar Flux @ "
                    + QString::number(mUpperFreq)
                    + " (MHz): "
                    + ui->lineEditSolarFluxHigh->text());
        mLogFile->append(
                    tr("\t\t\tHot\t\tCold"));
        mLogFile->append(
                    "Measurement 1:\t\t"
                    + ui->lineEditHot1->text()
                    + "\t\t"
                    + ui->lineEditCold1->text());
        mLogFile->append(
                    "Measurement 2:\t\t"
                    + ui->lineEditHot2->text()
                    + "\t\t"
                    + ui->lineEditCold2->text());
        mLogFile->append(
                    "Measurement 3:\t\t"
                    + ui->lineEditHot3->text()
                    + "\t\t"
                    + ui->lineEditCold3->text());
        mLogFile->append(
                    "Gain Over Temperature: "
                    + ui->lineEditGotOutput->text());
    }
}

/*----------------------------------------------------------------------------
Name         howTo

Purpose      Opens a window which explains how to perform a Gain Over
             Temperature calculation;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::howTo()
{
    mHowTo = new HowTo(this);
    mHowTo->setWindowModality(Qt::NonModal);
    mHowTo->show();
}

/*----------------------------------------------------------------------------
Name         options

Purpose      Opens an options window;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::options()
{
    mOptions = new OptionMenu(this);
    mOptions->setWindowModality(Qt::ApplicationModal);
    mOptions->show();
}

void MainWindow::about()
{
    mAbout = new About(this);
    mAbout->setWindowModality(Qt::NonModal);
    mAbout->show();
}

/*----------------------------------------------------------------------------
Name         checkGotFields

Purpose      Checks the Gain Over Temperature fields, within the Gain Over
             Temperature Group Box, for completion;

Returns      true  -  If all fields have been filled;
             false -  If a field is missing;

History		 11 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
bool MainWindow::checkGotFields()
{
    bool missingInfo = false;
    // OR the temporary bool variable with the isEmpty() function of each line-
    // edit.  Due to each line edit having an input mask/validator, it is
    // guaranteed that there will either be valid information entered, or none
    // at all; we are gaurding against the latter;
    missingInfo |= ui->lineEditAntennaFrequency->text().isEmpty();
    missingInfo |= ui->lineEditBeamWidth->text().isEmpty();
    missingInfo |= ui->lineEditSolarFluxLow->text().isEmpty();
    missingInfo |= ui->lineEditSolarFluxHigh->text().isEmpty();
    missingInfo |= ui->lineEditCold1->text().isEmpty();
    missingInfo |= ui->lineEditCold2->text().isEmpty();
    missingInfo |= ui->lineEditCold3->text().isEmpty();
    missingInfo |= ui->lineEditHot1->text().isEmpty();
    missingInfo |= ui->lineEditHot2->text().isEmpty();
    missingInfo |= ui->lineEditHot3->text().isEmpty();

    // If a line edit contains no information;
    if (missingInfo)
    {
        // Explain that more information must be entered;
        QMessageBox::critical(this
                              , "Critical"
                 , "Fill out all fields before running a calculation.");

        return false;
    }

    return true;
}

/*----------------------------------------------------------------------------
Name         loadSettings

Purpose      Loads any settings from the global settings;

History		 11 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::loadSettings()
{
    QSettings settings;
    if (settings.contains("DefaultSaveLoc"))
    {
        mDefaultSaveLoc = settings.value("DefaultSaveLoc").toString();
    }
}
