#ifndef SESSIONPAINTER_H
#define SESSIONPAINTER_H

#include <QPointF>
#include <QPainter>

class RunSession;
class BikeSession;

class SessionPainter {
public:
    virtual QPointF paint(const RunSession&,  const QPointF& offset, QPainter& painter) const = 0;
    virtual QPointF paint(const BikeSession&, const QPointF& offset, QPainter& painter) const = 0;
    //virtual QPointF paint(const DaySchedule&, const QPointF& offset, QPainter& painter) const = 0;
  //  virtual void visit(OtherSession *other);

protected:
    SessionPainter() {}
    virtual ~SessionPainter() {}
};

#endif // SESSIONPAINTER_H
