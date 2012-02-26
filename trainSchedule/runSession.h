#ifndef RUNSESSION_H
#define RUNSESSION_H

#include "sessionType.h"

#include <QXmlStreamReader>

class RunSession : public SessionType {
public:
    virtual int getTypeCode() const {
        return RUN;
    }
    virtual QString speed(int sec, double KM) const;
    virtual QPointF paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const;
    virtual QString toTestString() const;
   // virtual RunSession* clone() const;

    virtual void writeXml(QXmlStreamWriter& writer) const;
};

#endif // RUNSESSION_H
