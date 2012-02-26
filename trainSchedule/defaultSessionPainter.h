#ifndef DEFAULTSESSIONPAINTER_H
#define DEFAULTSESSIONPAINTER_H

#include "sessionPainter.h"

#include <QFont>
#include <QFontMetricsF>
#include <QString>
#include <QSize>

/* This class cares about painting different session types in default way
   Other SessionPainters are possible by deriving from SessionPainter
   and implementing paint methods for all session types
  */

class RunSession;
class BikeSession;

class DefaultSessionPainter : public SessionPainter
{
public:
    DefaultSessionPainter();
    ~DefaultSessionPainter() {}

    virtual QPointF paint(const RunSession& run, const QPointF& offset, QPainter& painter) const;
    virtual QPointF paint(const BikeSession& bike, const QPointF& offset, QPainter& painter) const;
//    virtual QPointF paint(const DaySchedule& schedule, const QPointF& offset, QPainter& painter) const;

private:
    int padding;

};

#endif // DEFAULTSESSIONPAINTER_H
