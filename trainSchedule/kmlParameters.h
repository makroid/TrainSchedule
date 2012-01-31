#ifndef KMLPARAMETERS_H
#define KMLPARAMETERS_H

class KmlParameters
{
public:
    KmlParameters();
    KmlParameters(int totDur, double altitude, double range);

    int durationInSec;
    double altitudeInMeter;
    double rangeInMeter;
};

#endif // KMLPARAMETERS_H
