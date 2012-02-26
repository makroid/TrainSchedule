#ifndef BIKESESSION_H
#define BIKESESSION_H

#include "sessionType.h"

#include <QXmlStreamReader>

class BikeSession : public SessionType {
public:

    virtual int getTypeCode() const {
        return BIKE;
    }
    virtual QString speed(int sec, double KM) const;
    virtual QPointF paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const;
    virtual QString toTestString() const;
   // virtual BikeSession* clone() const;

    virtual void writeXml(QXmlStreamWriter &writer) const;
};

#endif // BIKESESSION_H
