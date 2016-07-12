/*----------------------------------------------------------------------------
Name         solarcalc.cpp

Purpose      Given latitude, longitude, current time, and date calculate the
             altitude and azimuth of the sun;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/

#include "solarcalc.h"

/*----------------------------------------------------------------------------
Name         SolarCalc

Purpose      Constructor

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
SolarCalc::SolarCalc(QObject *parent) : QObject(parent)
{
    mTimeWasSet = false;
    mDateWasSet = false;
}

/*----------------------------------------------------------------------------
Name         SolarCalc

Purpose      Constructor

Input        rLatitude          The latitude of the unit;
             rLongitude         The longitude of the unit;
             time               The current time;
             date               The current date;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
SolarCalc::SolarCalc(const double &rLatitude
                     , const double &rLongitude
                     , QTime time, QDate date
                     , bool daylightSavings):
    mLatitudeDeg(rLatitude)
  , mLongitudeDeg(rLongitude)
  , mTime(time)
  , mDate(date)
  , mIsDaylightSavings(daylightSavings)
{
    mLatitudeRad = getRadians(mLatitudeDeg);
    mLongitudeRad = getRadians(mLongitudeDeg);

    mHour = mTime.hour();
    mMinute = mTime.minute();
    mDayOfYear = mDate.dayOfYear();

    mTimeWasSet = true;
    mDateWasSet = true;
}

/*----------------------------------------------------------------------------
Name         calculate

Purpose      Calculates the azimuth and elevation of the sun;

Notes        Because the calculation involes many minor calculations it's
             summed up entirely in this function.  This function takes the sub-
             function and calls them in an order which will guarantee a proper
             calculation as the result of some of them have a direct impact
             on the result of others (i.e. the Equation of Time is used in
             calculating the True Solar Time and therefore must be calculated
             first).

             Before running this function, the following functions should be
             called:
             - setTime
             - setDate
             - setDaylightSavings
             - setLongitude
             - setLatitude

             Alternatively, the constructor which has the applicable variables
             as arguments can be called;

History		 4 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculate()
{
     calculateEot(); // Calculates Equation of Time;
     calculateTst(); // Calculates True Solar Time;
     calculateHa(); // Calculates Hour Angle;
     calculateDec(); // Calculates Solar Declination;
     calculateZen(); // Calculates Solar Zenith;
     calculateAz(); // Calculates Solar Azimuth;
     calculateAlt(); // Calculates Solar Altitude (elevation);
}

/*----------------------------------------------------------------------------
Name         setTime

Purpose      Sets the current time used for calculation of the solar azimuth
             and altitude angles;

Input        rDate                  QTime with the time to be used to calculate
                                    the solar altitude/azimuth;

History		 4 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::setTime(const QTime& rTime)
{
    mTimeWasSet = true;

    mTime = rTime;
    mHour = mTime.hour();
    mMinute = mTime.minute();
}

/*----------------------------------------------------------------------------
Name         setDate

Purpose      Sets the current date used for calculation of the solar azimuth
             and altitude angles;

Input        rDate                  QDate with the day to be used to calculate
                                    the solar altitude/azimuth;

History		 4 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::setDate(const QDate& rDate)
{
    mDateWasSet = true;

    mDate = rDate;
    mDayOfYear = mDate.dayOfYear();
}

/*----------------------------------------------------------------------------
Name         setDaylightSavings

Purpose      Sets whether or not the location is currently in daylight savings
             time.

Input        rDst                   Boolean variable expressing:
                                    true - is currently daylight savings time
                                    false - is not currently dst;
Notes        This function was created due to a toss up between two different
             options:
             - The user has to know their time from GMT/UTC;
             - The user has to know their local time and whether or not it's
               daylight savings time.

             Personally, I think either way it requires the user to know
             extra information, but when the user is in an area familiar to
             them (such as operating this in California), I am much more likely
             to know if its daylight savings time, and therefore will not have
             to use a means of query to determine something, whereas I would
             most definitely have to determine my time against GMT no matter
             where I am.  That said, this entire thing could be removed if
             the time in the GUI were just asked as UTC or GMT versus local
             time.

History		 11 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::setDaylightSavings(const bool &rDst)
{
    mIsDaylightSavings = rDst;
}

/*----------------------------------------------------------------------------
Name         calculateEot

Purpose      Calculates an estimated Equation of Time based on the day of the
             year;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateEot()
{
    // If the date wasn't set by the user, use the current system date;
    if(!mDateWasSet)
    {
        setDate(QDate::currentDate());
    }

    double b = (360.0/365.0) * (mDayOfYear - 81.0);
    b = getRadians(b);

    mEquationOfTime = (9.87 * sin((2.0 * b)))
            - (7.53 * cos(b)) - (1.5 * sin(b));
}

/*----------------------------------------------------------------------------
Name         calculateTst

Purpose      Calculates the True Solar Time, which is a factor of the current
             local time, the Equation of Time, and the Longitude;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateTst()
{
    // Test if the time was set by the user.  If this is false, use the
    // computer's system time;
    if (!mTimeWasSet)
    {
        setTime(QTime::currentTime());
    }

    // Account for daylight savings time;
    if (mIsDaylightSavings)
    {
        mHour -= 1;
    }

    // Note that in the final portion of this equation:
    // mLongitudeDeg / 15
    // I am using a more precise calculation of the offset from UTC than if
    // I were to have user enter in their UTC offset;
    mTrueSolarTime = ((((mMinute / 60.0) + mHour) / 24.0) * 1440.0
                      + mEquationOfTime + 4.0
                      * mLongitudeDeg - 60.0
                      * (mLongitudeDeg / 15.0));

    mTrueSolarTime = mTrueSolarTime
            - (1440.0 * (floor(mTrueSolarTime/1440.0)));
}

/*----------------------------------------------------------------------------
Name         calculateHa

Purpose      Calculates the Hour Angle;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateHa()
{
    if ((mTrueSolarTime / 4.0) < 0)
    {
        mHourAngleDeg = mTrueSolarTime / 4.0 + 180.0;
    }

    else if ((mTrueSolarTime / 4.0) >= 0)
    {
        mHourAngleDeg = mTrueSolarTime / 4.0 - 180.0;
    }

    mHourAngleRad = getRadians(mHourAngleDeg);
}

/*----------------------------------------------------------------------------
Name         calculateDec

Purpose      Calculates the Solar Declination

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateDec()
{
    // 23.45 is the maximum declination over the period of Earth's rotation;
    mSolarDeclinationDeg = 23.45
            * sin(getRadians((360.0/365.0) * (181.0 - 81.0)));

    mSolarDeclinationRad = getRadians(mSolarDeclinationDeg);
}

/*----------------------------------------------------------------------------
Name         calculateZen

Purpose      Calculates the Solar Zenith

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateZen()
{
    mZenithRad  = acos(sin(mLatitudeRad)
                       * sin(mSolarDeclinationRad)
                       + cos(mLatitudeRad)
                       * cos(mSolarDeclinationRad)
                       * cos(mHourAngleRad));
    mZenithDeg = getDegrees(mZenithRad);
}


/*----------------------------------------------------------------------------
Name         calculateAlt

Purpose      Calculates the Solar Altitude

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateAlt()
{
    mSolarAltitudeDeg = 90.0 - getDegrees(mZenithRad);
}

/*----------------------------------------------------------------------------
Name         calculateAz

Purpose      Calculates the Solar Azimuth

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::calculateAz()
{

    if (mHourAngleDeg > 0)
    {
         mSolarAzimuthDeg = (getDegrees(acos((sin(mLatitudeRad)
                                              * cos(mZenithRad)
                                              - sin(mSolarDeclinationRad))
                                             /
                                              (cos(mLatitudeRad)
                                              * sin(mZenithRad))))) + 180.0;
    }

    else
    {
        mSolarAzimuthDeg = 540.0 - (getDegrees(acos((sin(mLatitudeRad)
                                                   * cos(mZenithRad)
                                                   - sin(mSolarDeclinationRad))
                                                  /
                                                  (cos(mLatitudeRad)
                                                   * sin(mZenithRad)))));
    }
    mSolarAzimuthDeg = mSolarAzimuthDeg - (360.0 * floor(mSolarAzimuthDeg/360.0));
}

/*----------------------------------------------------------------------------
Name         setLatitude

Purpose      Sets the latitude which will be used for calculations;

Input        rLatitude              Latitude in degrees;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::setLatitude(const double &rLatitude)
{
    mLatitudeDeg = rLatitude;
    mLatitudeRad = getRadians(mLatitudeDeg);
}

/*----------------------------------------------------------------------------
Name         setLongitude

Purpose      Sets the longitude which will be used for calculations;

Input        rLongitude             Longitude in degrees;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void SolarCalc::setLongitude(const double &rLongitude)
{
    mLongitudeDeg = rLongitude;
    mLongitudeRad = getRadians(mLongitudeDeg);
}

/*----------------------------------------------------------------------------
Name         getSolarAzimuth

Purpose      Returns the Solar Azimuth in degrees;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getSolarAzimuth()
{
    return mSolarAzimuthDeg;
}

/*----------------------------------------------------------------------------
Name         getSolarAltitude

Purpose      Returns the Solar Altitude in degrees;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getSolarAltitude()
{
    return mSolarAltitudeDeg;
}

/*----------------------------------------------------------------------------
Name         getEquationOfTime

Purpose      Returns the calculated Equation of Time;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getEquationOfTime()
{
    return mEquationOfTime;
}

/*----------------------------------------------------------------------------
Name         getTrueSolarTime

Purpose      Returns true solar time;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getTrueSolarTime()
{
    return mTrueSolarTime;
}

/*----------------------------------------------------------------------------
Name         getHourAngle

Purpose      Returns the Hour Angle in degrees;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getHourAngle()
{
    return mHourAngleDeg;
}

/*----------------------------------------------------------------------------
Name         getRadians

Purpose      Converts degrees to radians;

Input        degrees            double containing the desired value to convert
                                in degrees;

Returns      double             input converted to radians;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getRadians(double degrees)
{
    return (degrees * (M_PI / 180.0));
}

/*----------------------------------------------------------------------------
Name         getDegrees

Purpose      Converts radians to degrees;

Input        degrees            double containing the desired value to convert
                                in radians;

Returns      double             input converted to degrees;

History		 29 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
double SolarCalc::getDegrees(double radians)
{
    return (radians * (180.0 / M_PI));
}




