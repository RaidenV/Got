#ifndef SOLARCALC_H
#define SOLARCALC_H

#include <QObject> // ISA this object is a QObject;
#include <QTime> // HASA QTime object for keeping track of user-entered time;
#include <QDate> // HASA QDate object for keeping track of user-entered date;
#include <cmath> // USES several cmath functions;
#include <QDebug>

class SolarCalc : public QObject
{
    Q_OBJECT
public:
    explicit SolarCalc(QObject *parent = 0);
    explicit SolarCalc(const double& rLatitude
                       , const double& rLongitude
                       , QTime time
                       , QDate date);
    ~SolarCalc(){}

    void calculate(void); // Makes all necessary calculations;


    void setTime(const QTime& rTime); // Sets the current local time;
    void setDate(const QDate& rDate); // Sets the current date;
    void setLatitude(const double& rLatitude); // Sets the unit's latitude;
    void setLongitude(const double& rLongitude); // Sets the unit's longitude;

    double getSolarDeclination(void); // Returns the Solar Declination in Deg;
    double getSolarAzimuth(void); // Returns the Solar Azimuth;
    double getSolarAltitude(void); // Returns the Solar Altidude (elevation)
                                   // in degrees;
    double getSolarZenith(void); // Returns the Solar Zenith;
    double getEquationOfTime(void); // Returns the Equation of Time;
    double getTrueSolarTime(void); // Returns the Time Correction;
    double getHourAngle(void); // Returns the Hour Angle in degrees;

private:
    double mLatitudeDeg; // The unit's latitude;
    double mLatitudeRad;

    double mLongitudeDeg; // The unit's longitude;
    double mLongitudeRad;

    QTime mTime; // Current local time entered by user;
    QDate mDate; // Current date;
    bool mTimeWasSet;
    bool mDateWasSet;

    int mHour; // Current hour;
    int mMinute; // Current minute;
    int mDayOfYear; // Day of year (1-365 (366 for leap year));

    double mSolarDeclinationDeg;
    double mSolarDeclinationRad;

    double mSolarAzimuthDeg;

    double mSolarAltitudeDeg;

    double mZenithDeg;
    double mZenithRad;

    double mEquationOfTime;
    double mTrueSolarTime;

    double mHourAngleDeg;
    double mHourAngleRad;

    double mLocalStandardTimeMeridianDeg;

    void calculateEot(void); // Calculates Equation of Time;
    void calculateTst(void); // Calculates True Solar Time;
    void calculateHa(void); // Calculates Hour Angle;
    void calculateDec(void); // Calculates Solar Declination;
    void calculateZen(void); // Calculates the Zenith;
    void calculateAz(void); // Calculates Solar Azimuth;
    void calculateAlt(void); // Calculates Solar Altitude (elevation);

    double getRadians(double degrees);
    double getDegrees(double radians);
};

#endif // SOLARCALC_H
