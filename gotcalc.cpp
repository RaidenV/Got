/*----------------------------------------------------------------------------
Name         gotcalc.cpp

Purpose      Calculate Gain Over Temperature;

Notes        This calculation is based largely on work done by Richard Flagg,
             call sign AH6NM;

History		 7 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
#include "gotcalc.h"

GotCalc::GotCalc(QObject *parent) : QObject(parent)
{
    // Initialize all variables to zero;
    mSolarFluxPoint = 0;
    mSolarFluxHigh = 0;
    mSolarFluxLow = 0;

    mOperatingFrequencyMHz = 0;
    mHigherFreqMHz = 0;
    mLowerFreqMHz = 0;

    mHotAverage = 0;
    mColdAverage = 0;

    mBeamwidth = 0;
    mBeamCorrectionFactor = 0;

    mGotPure = 0;
    mGotdB = 0;
}

/*----------------------------------------------------------------------------
Name         calculate

Purpose      Calculates the Gain Over Temperature

Notes        The following variables are required for this calculation.  These
             variables are set by the user through the setter functions made
             available:

             - mSolarFluxLow
             - mSolarFluxHigh
             - mLowerFreqMHz
             - mHigherFreqMHz
             - mOperatingFrequency
             - mBeamwidth
             - All hot and cold measurements;

             The calculation solves for G/T and places the answer into two
             variables:

             - mGotPure is the pure ratio of G/T;
             - mGotdB is the pure ratio converted into decibel units according
               to 10 * log10(mGotPure), or dBW;

History		 10 Jul 16  AFB	Created
             18 Jul 16  AFB Update equation, converting the Y value from dB to
                            a power ratio;
----------------------------------------------------------------------------*/
void GotCalc::calculate()
{
    // Calculate the solar flux at a specific point given two frequencies
    // (the next frequency above the operating frequency and the frequency
    // directly beneath the operating frequency) and two solar flux values
    // (corresponding to the aforementioned frequencies);
    mSolarFluxPoint = linearInterpolation(mSolarFluxLow
                                               , mSolarFluxHigh
                                               , mLowerFreqMHz
                                               , mHigherFreqMHz
                                               , mOperatingFrequencyMHz);
    // Conver the newly obtained Solar Flux value to Watts per Meters
    // Squared per Hertz;
    mSolarFluxPoint *= constants::W_M2_Hz;

    // Get the wavelength at the operating frequency, in meters;
    mWavelengthm = constants::speed_of_light / mOperatingFrequencyMHz;

    // Get the average of the hot and cold measurements;
    mHotAverage = average(mHotMeasurements);
    mColdAverage = average(mColdMeasurements);

    // Get the Sun Noise Rise;
    double sunNoiseRise = pow(10.0, ((mHotAverage - mColdAverage)/10.0));

    // Get the Beam Correction Factor;
    mBeamCorrectionFactor = calculateBeamwidthCorrectionFactor();

    double wavelengthSquared = mWavelengthm * mWavelengthm;

    // Produce the numerator;
    double numerator = ((sunNoiseRise - 1.0)
                        * 8.0
                        * M_PI
                        * constants::boltzmann_constant
                        * mBeamCorrectionFactor);

    // Produce the denominator;
    double denominator = (mSolarFluxPoint * pow(mWavelengthm, 2.0));

    // Divide to get the Gain Over Temperature Value;
    mGotPure = numerator / denominator;

    // Convert into a decibel value;
    mGotdB = 10 * log10(mGotPure);
}

/*----------------------------------------------------------------------------
Name         getAvailableFrequencies

Purpose      Places the frequencies available for solar flux measurments into
             the argument std::vector;

Input        rFreq           std::vector to be loaded with available freqs;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::getAvailableFrequencies(std::vector<double> &rFreq)
{
    for(size_t i = 0; i < constants::number_of_available_frequencies; i++)
    {
        rFreq.push_back(constants::available_frequencies[i]);
    }
}

/*----------------------------------------------------------------------------
Name         getInterpolatedSolarFlux

Purpose      Returns the solar flux which was interpolated by the Gain Over
             Temperature calculation;

Returns      mSolarFluxPoint            The interpolated solar flux value;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
double GotCalc::getInterpolatedSolarFlux()
{
    return mSolarFluxPoint;
}

/*----------------------------------------------------------------------------
Name         getGotRatio

Purpose      Returns the pure Gain Over Temperature ratio;

Returns      mGotPure            Pure Gain Over Temperature ratio;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
double GotCalc::getGotRatio()
{
    return mGotPure;
}

/*----------------------------------------------------------------------------
Name         getGotRatiodB

Purpose      Returns the Gain Over Temperature in decibels;

Returns      mGotdB            Gain Over Temperature in decibels;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
double GotCalc::getGotRatiodB()
{
    return mGotdB;
}

/*----------------------------------------------------------------------------
Name         setSolarFluxHigh

Purpose      Sets the solar flux of the higher frequency;

Input        rFlux           Value of the solar flux of the higher frequency;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::setSolarFluxHigh(const double &rFlux)
{
    mSolarFluxHigh = rFlux;
}

/*----------------------------------------------------------------------------
Name         setSolarFluxLow

Purpose      Sets the solar flux of the lower frequency;

Input        rFlux           Value of the solar flux of the lower frequency;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::setSolarFluxLow(const double &rFlux)
{
    mSolarFluxLow = rFlux;
}

/*----------------------------------------------------------------------------
Name         setOperatingFrequency

Purpose      Sets the operating frequency, or the frequency at which the Gain
             over temperature will be calculated;

Input        rFreq           Value of the operating frequency, in MHz;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::setOperatingFrequency(const double &rFreq)
{
    mOperatingFrequencyMHz = rFreq;
}

/*----------------------------------------------------------------------------
Name         setHigherFrequency

Purpose      Sets the higher frequency for which the solar flux will be used;

Input        rFreq           Value of the higher frequency, in MHz;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::setHigherFrequency(const double &rFreq)
{
    mHigherFreqMHz = rFreq;
}

/*----------------------------------------------------------------------------
Name         setLowerFrequency

Purpose      Sets the lower frequency for which the solar flux will be used;

Input        rFreq           Value of the lower frequency, in MHz;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::setLowerFrequency(const double &rFreq)
{
    mLowerFreqMHz = rFreq;
}

void GotCalc::setBeamwidth(const double &rBeamwidth)
{
    mBeamwidth = rBeamwidth;
}

/*----------------------------------------------------------------------------
Name         addHotMeasurment

Purpose      Adds another hot measurement to the mHotMeasurements vector;

Input        rMeasurement           Value of the measurement;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::addHotMeasurement(const double &rMeasurement)
{
    mHotMeasurements.push_back(rMeasurement);
}

/*----------------------------------------------------------------------------
Name         addColdMeasurement

Purpose      Adds another cold measurement to the mColdMeasurements vector;

Input        rMeasurement           Value of the measurement;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::addColdMeasurement(const double &rMeasurement)
{
    mColdMeasurements.push_back(rMeasurement);
}

/*----------------------------------------------------------------------------
Name         clearHotMeasurments

Purpose      Clears the mHotMeasurements vector;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::clearHotMeasurments()
{
    mHotMeasurements.clear();
}

/*----------------------------------------------------------------------------
Name         clearColdMeasurments

Purpose      Clears the mColdMeasurements vector;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
void GotCalc::clearColdMeasurments()
{
    mColdMeasurements.clear();
}

double GotCalc::calculateBeamwidthCorrectionFactor()
{
    double radioSunDiameter = 0;
    if (mBeamwidth >= 3)
        return 1;

    else
    {
        if (mOperatingFrequencyMHz > 400)
        {
            radioSunDiameter = 0.7;
        }

        if (mOperatingFrequencyMHz > 1420)
        {
            radioSunDiameter = 0.6;
        }

        if (mOperatingFrequencyMHz > 3000)
        {
            radioSunDiameter = 0.5;
        }

        return (1 + 0.38 * pow((radioSunDiameter / mBeamwidth), 2.0));
    }
}

/*----------------------------------------------------------------------------
Name         linearInterpolation

Purpose      Performs a linear interpolation of the point xPoint given two
             (x,y) coordinates on a Cartesian plane;

Inputs        y1           y value of the first coordinate;
              y2           y value of the second coordinate;
              x1           x value of the first coordinate;
              x2           x value of the second coordinate;
              xPoint       The x value for which a y value will be interpolated

Returns       yResult      The y value which has been linearly
                           interpolated from the x value xPoint;

Notes        Solves for a simple y = m(x) + b linear equation;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
double GotCalc::linearInterpolation(double y1
                                    , double y2
                                    , double x1
                                    , double x2
                                    , double xPoint)
{
    double  x = 0, b = 0, yResult = 0;

    x = (y2 - y1) / (x2 - x1);
    b = (y1)  - (x1 * x);
    yResult = xPoint * x + b;

    return yResult;
}

/*----------------------------------------------------------------------------
Name         exponentialInterpolation

Purpose      Performs an exponential interpolation of the point xPoint given
             two (x,y) coordinates on a Cartesian plane;

Inputs        y1           y value of the first coordinate;
              y2           y value of the second coordinate;
              x1           x value of the first coordinate;
              x2           x value of the second coordinate;
              xPoint       The x value for which a y value will be interpolated

Returns       yResult      The y value which has been exponentially
                           interpolated from the x value xPoint;

Notes        Solves for a simple y = A * e ^ (kt) exponential equation;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
double GotCalc::exponentialInterpolation(double y1
                                         , double y2
                                         , double x1
                                         , double x2
                                         , double xPoint)
{
    double A = 0, k = 0, yResult = 0;

    k = log(y2 / y1) / (x2 - x1);
    A = y2 / exp(x2 * k);

    yResult = A * exp(xPoint * k);

    return yResult;
}

/*----------------------------------------------------------------------------
Name         average

Purpose      Takes the average of a set of values;

Inputs       values             std::vector of doubles for which an average
                                will be calculated;

Returns      avg                The average of the std::vector values;

History		 10 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
double GotCalc::average(const std::vector<double>& values)
{
    double avg = 0;

    for(size_t i = 0; i < values.size(); i++)
    {
        avg += values.at(i);
    }

    return (avg /= values.size());
}

