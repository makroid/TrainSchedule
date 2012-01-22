#ifndef SESSIONTYPE_H
#define SESSIONTYPE_H

#include <QSharedPointer>

class QPointF;
class SessionPainter;
class QPainter;
class QString;
class QXmlStreamWriter;
class Session;

/* This abstract base class provides SessionType specific
   behaviour by delegating all functions to the apropriate
   derived classes.
  */

class SessionType {
public:
    virtual ~SessionType() {}
    // for creating/changing the type
    static QSharedPointer<SessionType> newType(int code);

    // needed by some GUI functions
    virtual int getTypeCode() const = 0;
    // session type-specific speed (e.g. min/km, kmh)
    virtual QString speed(int sec, double KM) const = 0;
    // SessionPainter specifies the type of painting, eg. default
    virtual QPointF paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const = 0;
    virtual QString toTestString() const = 0;
    virtual void writeXml(QXmlStreamWriter& writer) const = 0;

    // must have access to data in Session
    Session* getSession() const { return session; }
    void setSession(Session* asession) { session = asession; }

    static const int RUN = 0;
    static const int BIKE = 1;

protected:
    Session* session;
};

#endif // SESSIONTYPE_H
