/*----------------------------------------------------------------------------
Name         gotcalc.h

Purpose      Calculate Gain Over Temperature;

History		 7 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
#ifndef GOTCALC_H
#define GOTCALC_H

#include <QObject> // ISA QObject
#include <cmath> // USES many math functions;
#include <QDebug>

// Necessary constants;
namespace constants
{
    const double boltzmann_constant = 1.3806503e-23;

    // Speed of light in megameters per second;
    const double speed_of_light = 299.792458;

    // Conversion factor from Solar Units to Watts per Meter Squared per Hertz;
    const double W_M2_Hz = 10e-23;

    // Number of frequencies which are available for Solar Flux Data;
    const int number_of_available_frequencies = 9;

    // Frequencies for which Solar Flux Data might be provided;
    const double available_frequencies[number_of_available_frequencies]
        = {245, 410, 610, 1415, 2695, 2800, 4995, 8800, 15400};

}

class GotCalc : public QObject
{
    Q_OBJECT

public:
    explicit GotCalc(QObject *parent); // Constructor;

    ~GotCalc(){} // Destructor;

    void calculate(); // Calculates the G Over T value;

    // Returns frequencies for which Solar Flux can be gathered;
    void getAvailableFrequencies(std::vector<double>& rFreq);
    // Returns the interpolated Solar Flux value;
    double getInterpolatedSolarFlux(void);
    // Returns Gain Over Temperature as a pure ratio;
    double getGotRatio(void);
    // Returns Gain Over Temperature in dB;
    double getGotRatiodB(void);

    // Sets the Solar Flux of the higher frequency;
    void setSolarFluxHigh(const double& rFlux);
    // Sets the Solar Flux of the lower frequency;
    void setSolarFluxLow(const double& rFlux);

    // Sets the frequency at which the antenna is operating;
    void setOperatingFrequency(const double& rFreq);
    // Sets the next frequency higher than operating;
    void setHigherFrequency(const double& rFreq);
    // Sets the next frequency lower than operating;
    void setLowerFrequency(const double& rFreq);

    // Sets the beamwidth of the antenna;
    void setBeamwidth(const double& rBeamwidth);

    // Adds a hot measurement to the mHotMeasurements vector;
    void addHotMeasurement(const double& rMeasurement);
    // Adds a cold measurement to the mColdMeasurements vector;
    void addColdMeasurement(const double& rMeasurement);

    // Clears the mHotMeasurements vector;
    void clearHotMeasurments(void);
    // Clears the mColdMeasurements vector;
    void clearColdMeasurments(void);

private:
    double mSolarFluxPoint; // Interpolated solar flux value;
    double mSolarFluxHigh; // Solar flux of the higher frequency;
    double mSolarFluxLow; // Solar flux of the lower frequency;

    double mOperatingFrequencyMHz; // The frequency at which the antenna works;
    double mHigherFreqMHz; // Higher frequency used in interpolation;
    double mLowerFreqMHz; // Lower frequency used in interpolation;
    double mWavelengthm; // Wavelength of frequency in meters;

    // Vector of the hot measurements entered by the user, in dB;
    std::vector<double> mHotMeasurements;
    // Vector of the cold measurements entered by the user, in dB;
    std::vector<double> mColdMeasurements;

    // Average of the measurements taken while pointing at the sun;
    double mHotAverage;
    // Average of the measureents taken while pointing away from the sun;
    double mColdAverage;

    // Beamwidth of the antenna;
    double mBeamwidth;
    // Beamwidth correction factor;
    double mBeamCorrectionFactor;

    // Gain Over Temperature as a pure ratio between sun noise power ratio,
    // frequency, beamwidth, and several other factors;
    double mGotPure;
    // Gain Over Temperature in dB, the value which will be returned;
    double mGotdB;

    // Calculates the beamwidth correction factor;
    double calculateBeamwidthCorrectionFactor();

    // Allows for linear interpolation of a solar flux value given two
    // (x, y) coordinates and the x value for which the y will be interpolated;
    double linearInterpolation(double y1
                               , double y2
                               , double x1
                               , double x2
                               , double xPoint);

    // Allows for exponential interpolation of a solar flux value given two
    // (x, y) coordinates and the x value for which the y will be interpolated;
    double exponentialInterpolation(double y1
                                    , double y2
                                    , double x1
                                    , double x2
                                    , double xPoint);

    // Returns an average value given a vector of doubles;
    double average(const std::vector<double>& values);
};

#endif // GOTCALC_H
