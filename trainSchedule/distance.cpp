#include "distance.h"
#include <QtDebug>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "helpers.h"

Distance::Distance() : dist(0.0), unit(KM)
{}

Distance::Distance(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "dist") {               
                dist = xmlReader.readElementText().toDouble();
                xmlReader.readNext();
            } else if (xmlReader.name() == "unit") {
                QString u = xmlReader.readElementText();
                if (u == "KM") {
                    unit = KM;
                } else if (u == "MILE") {
                    unit = MILE;
                } else {
                    unit = KM;
                }
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

Distance::Distance(const Distance &other)
    : dist(other.dist),
      unit(other.unit)
{}

Distance::Distance(double d) : dist(d), unit(KM)
{}

void Distance::kmToMile() {
    if (unit != KM) {
        qDebug() << "Warning: Distance::kmToMile: wrong unit!";
    }
    dist *= 0.621371192;
    unit = MILE;
}

void Distance::mileToKm() {
    if (unit != MILE) {
        qDebug() << "Warning: Distance::mileToKm: wrong unit!";
    }
    dist /= 0.621371192;
    unit = KM;
}

void Distance::kmToMeter() {
    if (unit != KM) {
        qDebug() << "Warning: Distance::kmToMeter: wrong unit!";
    }
    dist *= 1000;
    unit = METER;
}

void Distance::mileToMeter() {
    if (unit != MILE) {
        qDebug() << "Warning: Distance::mileToMeter: wrong unit!";
    }
    dist /= 0.621371192;
    dist *= 1000;
    unit = METER;
}

QString Distance::toTestString() const {
    QString result;
    result += QString("Distance=%1").arg(dist);
    switch(unit) {
        case METER: result += "m"; break;
        case KM: result += "km"; break;
        case MILE: result += "mi"; break;
    }
    return result;
}

QString Distance::toString() const {
    QString result;
    result += QString::number(dist);
    result += unitToQString(unit);
    return result;
}

QString Distance::unitToQString(Unit u) {
    switch(u) {
        case METER: return QString("m");
        case KM: return QString("km");
        case MILE: return QString("mi");
        default: return QString("km");
    }
}

void Distance::writeXml(QXmlStreamWriter& writer) const {
    writer.writeStartElement("distance");
    writer.writeTextElement("dist", QString::number(dist));
    writer.writeTextElement("unit", unitToQString(unit));
    writer.writeEndElement();
}

const Distance operator+(const Distance& lhs, const Distance&rhs) {
    Distance ret(lhs);
    ret += rhs;
    return ret;
}

const Distance operator-(const Distance& lhs, const Distance&rhs) {
    Distance ret(lhs);
    ret -= rhs;
    return ret;
}
