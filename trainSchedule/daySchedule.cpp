#include "daySchedule.h"
#include "helpers.h"

#include <QtDebug>
#include <QFont>
#include <QPainter>
#include <QFontMetricsF>
#include <QSharedPointer>
#include <QPointF>

DaySchedule::DaySchedule(QDate adate) : date(adate)
{}

DaySchedule::DaySchedule(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "date") {
                QString dateStr = xmlReader.readElementText();
                date = QDate::fromString(dateStr, "ddd dd.MM.yyyy");
                xmlReader.readNext();
            } else if (xmlReader.name() == "sessions") {
                readSessionsXML(xmlReader);
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void DaySchedule::readSessionsXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "session") {
                QSharedPointer<Session> rs = QSharedPointer<Session>(new Session(xmlReader));
                addSession(rs);
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void DaySchedule::addSession(QSharedPointer<Session> asession) {
    sessions.push_back(asession);
    emit newSessionAdded(sessions.size()-1);
}

bool DaySchedule::deleteSession(int aidx) {
    if (aidx < 0 || aidx >= sessions.length()) {
        return false;
    }
    sessions.removeAt(aidx);
    emit sessionDeleted(aidx);
    return true;
}

QString DaySchedule::toTestString() const {
    QString result;
    result += "DaySchedule:\n";
    result += "date=" + date.toString() + "\n";
    result += "sessions:\n";
    foreach (QSharedPointer<Session> sp, sessions) {
        result += sp->toTestString();
    }
    return result;
}

QPointF DaySchedule::paint(const SessionPainter& spainter, const QPointF& aoffset, QPainter& apainter) const {
    QPointF curPos = aoffset;

    // paint date
    QFont serifFont("Times", 12);
    QFontMetricsF fm(serifFont);
    int fontPixHeight = fm.height();
    apainter.setFont(serifFont);
    curPos += QPointF(0, fontPixHeight);
    apainter.drawText(curPos, date.toString("ddd dd.MM.yy"));
    curPos += QPointF(0, 15);

    constSessionIter citer = sessions.constBegin();
    for( ; citer != sessions.end(); citer++) {
        curPos = (*citer)->paint(spainter, curPos, apainter);
    }

    return curPos;
//    for (size_t i=0; i<sessions.size(); i++) {
//        curPos = sessions.at(i)->paint(spainter, curPos, apainter);
//    }
}

void DaySchedule::update() {
    emit sessionChanged(0);
}

void DaySchedule::writeXml(QXmlStreamWriter &writer) {
    writer.writeStartElement("dayschedule");
    // must write date in yyyy format because otherwise cant be parsed with fromString
    writer.writeTextElement("date", date.toString("ddd dd.MM.yyyy"));
    writer.writeStartElement("sessions");
    for (int i=0; i<sessions.length(); ++i) {
        sessions.value(i)->writeXml(writer);
    }
    writer.writeEndElement();
    writer.writeEndElement();
}
