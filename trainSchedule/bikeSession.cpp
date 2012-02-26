#include "bikeSession.h"
#include "sessionPainter.h"
#include "session.h"

//BikeSession* BikeSession::clone() const {
//    return new BikeSession(*this);
//}

QString BikeSession::speed(int sec, double KM) const {
    if (sec <= 0 || sec >= 3600*10 || KM > 400 || KM < 0) {
        return QString("undef");
    } else {
        double speedKmh = KM * 3600.0 / sec;
        return QString("%1km/h").arg(speedKmh, 0, 'f', 2);
    }
}

QPointF BikeSession::paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const {
    return spainter.paint(*this, offset, painter);
}

QString BikeSession::toTestString() const {
    QString result = "";
    result += "BikeSession:\n";
    result += "duration=" + session->getDuration().toString() + "\n";
    result += "distance=" + session->getDistance().toTestString() + "\n";

    return result;
}

void BikeSession::writeXml(QXmlStreamWriter &writer) const {
    writer.writeStartElement("session");
    writer.writeTextElement("type", "bike");
    session->getDuration().writeXml(writer);
    session->getDistance().writeXml(writer);
    writer.writeTextElement("description", session->getDescription());
    writer.writeTextElement("name", session->getName());
    session->getMapRoute().writeXml(writer);
    writer.writeEndElement();
}
