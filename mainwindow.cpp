#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Gain Over Temperature Calculator");

    mSolarCalc = new SolarCalc(this);
    mGotCalc = new GotCalc(this);

    ui->timeEdit->setTime(QTime::currentTime());
    ui->dateEdit->setDate(QDate::currentDate());

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
    ui->lineEditAntennaFrequency->setValidator(new QIntValidator(0, 50000, this));

    connect(ui->pushButtonCalculateSolarAzAlt, SIGNAL(clicked(bool))
            , this, SLOT(calculateSolarAzAlt()));
    connect(ui->pushButtonCalculateGot, SIGNAL(clicked())
            , this, SLOT(calculateGot()));
    connect(ui->lineEditAntennaFrequency, SIGNAL(editingFinished())
            , this, SLOT(setFrequencies()));
    connect(ui->actionHowTo, SIGNAL(triggered()), this, SLOT(howTo()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateSolarAzAlt()
{
    mSolarCalc->setLatitude(ui->lineEditLatitude->text().toDouble());
    mSolarCalc->setLongitude(ui->lineEditLongitude->text().toDouble());

    QTime rTime(ui->timeEdit->time());
    QDate rDate(ui->dateEdit->date());

    mSolarCalc->setTime(rTime);
    mSolarCalc->setDate(rDate);
    mSolarCalc->calculate();

    std::vector<double> frequencies;
    mGotCalc->getAvailableFrequencies(frequencies);

    ui->lineEditSolarAltitude->setText(
                QString::number(mSolarCalc->getSolarAltitude()));
    ui->lineEditSolarAzimuth->setText(
                QString::number(mSolarCalc->getSolarAzimuth()));
}

void MainWindow::calculateGot()
{
    setFrequencies();

    mGotCalc->clearHotMeasurments();
    mGotCalc->clearColdMeasurments();

    mGotCalc->addHotMeasurement(ui->lineEditHot1->text().toDouble());
    mGotCalc->addHotMeasurement(ui->lineEditHot2->text().toDouble());
    mGotCalc->addHotMeasurement(ui->lineEditHot3->text().toDouble());

    mGotCalc->addColdMeasurement(ui->lineEditCold2->text().toDouble());
    mGotCalc->addColdMeasurement(ui->lineEditCold2->text().toDouble());
    mGotCalc->addColdMeasurement(ui->lineEditCold2->text().toDouble());

    mGotCalc->setSolarFluxHigh(ui->lineEditSolarFluxHigh->text().toDouble());
    mGotCalc->setSolarFluxLow(ui->lineEditSolarFluxLow->text().toDouble());

    mGotCalc->setBeamwidth(ui->lineEditBeamWidth->text().toDouble());

    mGotCalc->calculate();

    ui->lineEditGotOutput->setText(QString::number(mGotCalc->getGotRatiodB()));
}

void MainWindow::setFrequencies()
{
    std::vector<double> frequencyList;
    mGotCalc->getAvailableFrequencies(frequencyList);

    double lowerFrequency = 0;
    double higherFrequency = 0;

    // The frequency which was entered by the user;
    double targetFrequency = ui->lineEditAntennaFrequency->text().toDouble();

    if (targetFrequency < frequencyList.at(0)
            || targetFrequency > frequencyList.at(frequencyList.size() - 1))
    {
        QMessageBox::critical(this, "Critical"
                              , ("Please enter a frequency between "
                                 + QString::number(frequencyList.at(0))
                                 + " and "
                                 + QString::number(frequencyList.at(frequencyList.size() - 1))));
        return;
    }

    for (size_t i = 0; i < frequencyList.size(); i++)
    {
        if (targetFrequency < frequencyList.at(i) && higherFrequency == 0)
        {
            higherFrequency = frequencyList.at(i);
            if (higherFrequency != 0)
            {
               lowerFrequency = frequencyList.at(i - 1);
            }
        }
    }

    ui->labelSolarFluxUpper->setText(
                "Solar Flux @ " + QString::number(higherFrequency) + " MHz");
    ui->labelSolarFluxLower->setText(
                "Solar Flux @ " + QString::number(lowerFrequency) + " MHz");

    mGotCalc->setOperatingFrequency(targetFrequency);
    mGotCalc->setHigherFrequency(higherFrequency);
    mGotCalc->setLowerFrequency(lowerFrequency);
}

void MainWindow::howTo()
{
    mHowTo = new HowTo(this);
    mHowTo->setWindowModality(Qt::NonModal);
    mHowTo->show();
}
