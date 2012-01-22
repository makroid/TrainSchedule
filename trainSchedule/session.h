#ifndef SESSION_H
#define SESSION_H

#include <QIcon>
#include <QString>
#include <QPointF>
#include <QPainter>
#include <QSharedPointer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "duration.h"
#include "distance.h"
#include "mapRoute.h"
#include "sessionType.h"

class SessionPainter;

/* Session is the main class for all session data:
   - distance, duration, ...
   - SessionType (bike, run, ...)
   SessionType is a (state) member of Session so that it is
   easier to change session-type.
   SessionType is an abstract base class for all types of
   different session types, which provide session specific behaviour.
   paint, writeXml, ... are forwarded to the apropriate SessionType functions
   BUT: it is important for SessionType to have access to Session data
        thus there must be a Sessions pointer in SessionType
   NOTE: having a base class Session and deriving classes BikeSession,
   RunSession, ... is also an option but maybe less flexible and more
   cumbersome to handle (eg. when changing session-type)
  */
class Session {
public:
    Session();
    Session(int type, Duration, Distance);
    Session(QXmlStreamReader&);
    virtual ~Session() {}

    // getters
    Duration getDuration() const {
        return duration;
    }
    Distance getDistance() const {
        return distance;
    }
    QString getDescription() const {
        return description;
    }
    QString getName() const {
        return name;
    }
    MapRoute& getMapRoute() {
        return mapRoute;
    }

    // setters
    void setType(int code) {
        type = SessionType::newType(code);
        type->setSession(this);
    }

    void setDuration(Duration aduration) {
        duration = aduration;
    }
    void setDistance(Distance adistance) {
        distance = adistance;
    }

    void setDescription(QString adescription) {
        description = adescription;
    }
    void setName(QString aname) {
        name = aname;
    }

    // utilities
    void addDistance(Distance adist) {
        distance += adist;
    }

    void addDuration(Duration adur) {
        duration += adur;
    }

    // forwarding methods
    QString speed(int sec, double KM) {
        return type->speed(sec, KM);
    }

    int getTypeCode() const {
        return type->getTypeCode();
    }

    QPointF paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const {
        return type->paint(spainter, offset, painter);
    }

    QString toTestString() const {
        return type->toTestString();
    }

    void writeXml(QXmlStreamWriter& writer) const {
        type->writeXml(writer);
    }

        //virtual Session* clone() const = 0;

protected:
    QSharedPointer<SessionType> type;
    Duration duration;
    Distance distance;
    QString description;
    QString name;
    MapRoute mapRoute;
};

#endif // SESSION_H
