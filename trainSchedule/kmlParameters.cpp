#include "kmlParameters.h"

KmlParameters::KmlParameters()
    : durationInSec(10),
      altitudeInMeter(1000.0),
      rangeInMeter(500.0)
{
}

KmlParameters::KmlParameters(int dur, double alt, double ran)
    : durationInSec(dur),
      altitudeInMeter(alt),
      rangeInMeter(ran)
{}
