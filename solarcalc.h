/*----------------------------------------------------------------------------
Name         solarcalc.h

Purpose      Given latitude, longitude, current time, and date calculate the
             altitude and azimuth of the sun;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/

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
    explicit SolarCalc(QObject *parent = 0); // Constructor;
    explicit SolarCalc(const double& rLatitude
                       , const double& rLongitude
                       , QTime time
                       , QDate date
                       , bool daylightSavings); // Constructor;

    ~SolarCalc(){} // Nothing on stack, nothing to destroy;

    void calculate(void); // Makes all necessary calculations;


    void setTime(const QTime& rTime); // Sets the current local time;
    void setDate(const QDate& rDate); // Sets the current date;
    void setDaylightSavings(const bool& rDst); // Sets daylight savings;
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
    bool mTimeWasSet; // Variable for holding whether or not the time was set;
    bool mDateWasSet; // Variable for holding whether or not the date was set;

    int mHour; // Current hour;
    int mMinute; // Current minute;
    int mDayOfYear; // Day of year (1-365 (366 for leap year));
    bool mIsDaylightSavings; // Whether or not it is currently DST in locale;

    double mSolarDeclinationDeg; // Solar Declination in Degrees;
    double mSolarDeclinationRad; // Solar Declination in Radians;

    double mSolarAzimuthDeg; // Solar Azimuth in Degrees;
    double mSolarAltitudeDeg; // Solar Altitude in Degrees;

    double mZenithDeg; // Solar Zenith in Degrees;
    double mZenithRad; // Solar Zenith in Radians;

    double mEquationOfTime; // Equation of time;
    double mTrueSolarTime; // True Solar Time;

    double mHourAngleDeg; // Hour Angle in Degrees;
    double mHourAngleRad; // Hour Angle in Radians;

    double mLocalStandardTimeMeridianDeg;

    void calculateEot(void); // Calculates Equation of Time;
    void calculateTst(void); // Calculates True Solar Time;
    void calculateHa(void); // Calculates Hour Angle;
    void calculateDec(void); // Calculates Solar Declination;
    void calculateZen(void); // Calculates the Zenith;
    void calculateAz(void); // Calculates Solar Azimuth;
    void calculateAlt(void); // Calculates Solar Altitude (elevation);

    double getRadians(double degrees); // Returns radians from degrees;
    double getDegrees(double radians); // Returns degrees from radians;
};

#endif // SOLARCALC_H
