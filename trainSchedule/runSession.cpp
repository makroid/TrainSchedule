#include "runSession.h"
#include "sessionPainter.h"
#include "session.h"

#include <QtDebug>
//RunSession* RunSession::clone() const {
//    return new RunSession(*this);
//}

QString RunSession::speed(int sec, double KM) const {
    Duration minKM;
    double speedKmh = -1;
    if (KM > 0.001 && KM < 1000 && sec >= 1) {
        speedKmh = KM / (sec / 3600.0);
        qDebug() << "speedKmh=" << speedKmh;
        int secPerKM = static_cast<int>(3600.0 / speedKmh);
        Duration tmp(secPerKM);
        minKM = tmp;
    }

    if (speedKmh == -1) {
        return QString("undef");
    } else {
        QString minPerKM = minKM.toString() + "/km";
        return minPerKM;
    }
}

QPointF RunSession::paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const {
    return spainter.paint(*this, offset, painter);
}

QString RunSession::toTestString() const {
    QString result = "";
    result += "RunSession:\n";
    result += "duration=" + session->getDuration().toString() + "\n";
    result += "distance=" + session->getDistance().toTestString() + "\n";
    return result;
}

void RunSession::writeXml(QXmlStreamWriter &writer) const {
    writer.writeStartElement("session");
    writer.writeTextElement("type", "run");
    session->getDuration().writeXml(writer);
    session->getDistance().writeXml(writer);
    writer.writeTextElement("description", session->getDescription());
    writer.writeTextElement("name", session->getName());
    session->getMapRoute().writeXml(writer);
    writer.writeEndElement();
}
