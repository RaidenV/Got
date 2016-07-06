#ifndef GOTCALC_H
#define GOTCALC_H

#include <QObject>
#include <cmath>

namespace constants
{
    const double boltzmann_constant = 1.3806503e-23;

    // Speed of light in megameters per second;
    const double speed_of_light = 299.792458;

    // Conversion factor from Solar Units to Watts per Meter Squared per Hertz;
    const double W_M2_Hz = 10e-23;
}

class GotCalc : public QObject
{
    Q_OBJECT

public:
    GotCalc();

    ~GotCalc(){}

private:
    double mSolarFluxHigh;
    double mSolarFluxLow;
    double mSolorFluxPoint;

    double mOperationalFrequencyMHz;
    double mLowerFreqMHz;
    double mHigherFreqMHz;

    std::vector<double> mHotMeasurements;
    std::vector<double> mColdMeasurements;

    double mHotAverage;
    double mColdAverage;

    double linearInterpolation(double y1
                               , double y2
                               , double x1
                               , double x2
                               , double xPoint);

    double exponentialInterpolation(double y1
                                    , double y2
                                    , double x1
                                    , double x2
                                    , double xPoint);
};

#endif // GOTCALC_H
