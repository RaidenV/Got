#include "gotcalc.h"

GotCalc::GotCalc()
{

}

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

