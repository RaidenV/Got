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

    ui->lineEditLatitude->setValidator(&mLatValid);
    ui->lineEditLongitude->setValidator(&mLonValid);

    connect(ui->pushButtonCalculateSolarAzAlt, SIGNAL(clicked(bool))
            , this, SLOT(calculateSolarAzAlt()));
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

    QTime rtime(ui->timeEdit->time());
    QDate rdate(ui->dateEdit->date());

    mSolarCalc->setDate(rdate);
    mSolarCalc->setTime(rtime);
    mSolarCalc->calculate();

    std::vector<double> frequencies;
    mGotCalc->getAvailableFrequencies(frequencies);

    ui->lineEditSolarAltitude->setText(QString::number(mSolarCalc->getSolarAltitude()));
    ui->lineEditSolarAzimuth->setText(QString::number(mSolarCalc->getSolarAzimuth()));
}

void MainWindow::howTo()
{
    mHowTo = new HowTo(this);
    mHowTo->setWindowModality(Qt::NonModal);
    mHowTo->show();
}
