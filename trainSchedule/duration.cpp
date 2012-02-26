#include "duration.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "helpers.h"

Duration::Duration() : hour(0), min(0), sec(0)
{}

Duration::Duration(int inSecs) {
    hour = 0;
    min = 0;
    sec = inSecs;
    toSixty();
}

Duration::Duration(int ahour, int amin, int asec) : hour(ahour), min(amin), sec(asec)
{
    toSec();
    toSixty();
}

Duration::Duration(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "hour") {
                hour = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else if (xmlReader.name() == "min") {
                min = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else if (xmlReader.name() == "sec") {
                sec = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
    toSixty();
}

Duration::Duration(const Duration& dur) : hour(dur.hour), min(dur.min), sec(dur.sec)
{}

void Duration::toMin() {
    min += hour*60;
    hour = 0;
    if (sec > 30) min++;
    sec = 0;
}

void Duration::toSec() {
    sec += hour*3600;
    hour = 0;
    sec += min*60;
    min = 0;
}

void Duration::toSixty() {
    int minsInSecs = static_cast<int>(sec/60);
    min += minsInSecs;
    sec -= minsInSecs*60;
    int hoursInMins = static_cast<int>(min/60);
    hour += hoursInMins;
    min -= hoursInMins*60;
}

// TODO: implement format options
QString Duration::toString(const QString& /*format*/) const {
    QString result = "";
    QString ending = "min";
    if (hour > 0) {
        ending = "h"; 
        result += QString("%1:").arg(hour);
    }

    if (min < 10)  {
        result += QString("0%1:").arg(min);
    } else {
        result += QString("%1:").arg(min);
    }

    if (sec < 10) {
        result += QString("0%1").arg(sec);
    } else {
        result += QString("%1").arg(sec);
    }
    return result + ending;
}

Duration Duration::operator+(const Duration& adur) const {
    Duration adddur(hour+adur.hour, min+adur.min, sec+adur.sec);
    return adddur;
}

// TODO: check negative durations?
Duration Duration::operator-(const Duration& adur) const {
    Duration subdur(hour-adur.hour, min-adur.min, sec-adur.sec);
    return subdur;
}

Duration& Duration::operator+=(const Duration& rhs) {
    hour += rhs.hour;
    min += rhs.min;
    sec += rhs.sec;
    toSixty();
    return (*this);
}

Duration& Duration::operator-=(const Duration& rhs) {
    hour -= rhs.hour;
    min -= rhs.min;
    sec -= rhs.sec;
    toSec();
    toSixty();

    return (*this);
}

Duration& Duration::operator =(const Duration& rhs) {
    if (this == &rhs) {
        return (*this);
    }
    hour = rhs.hour;
    min = rhs.min;
    sec = rhs.sec;

    return (*this);
}

void Duration::writeXml(QXmlStreamWriter &writer) const {
    writer.writeStartElement("duration");
    writer.writeTextElement("hour", QString::number(hour));
    writer.writeTextElement("min", QString::number(min));
    writer.writeTextElement("sec", QString::number(sec));
    writer.writeEndElement();
}
