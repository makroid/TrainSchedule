#include "sessionType.h"

#include "bikeSession.h"
#include "runSession.h"

QSharedPointer<SessionType> SessionType::newType(int acode) {
    switch(acode) {
        case BIKE: return QSharedPointer<SessionType>(new BikeSession());
        case RUN: return QSharedPointer<SessionType>(new RunSession());
        default: return QSharedPointer<SessionType>(new RunSession());
    }
}
