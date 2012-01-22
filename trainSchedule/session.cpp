#include "session.h"
#include "helpers.h"

#include <QtDebug>

Session::Session() {
}

Session::Session(int atype, Duration adur, Distance adist)
    : duration(adur),
      distance(adist),
      description("")
{
    setType(atype);
    type->setSession(this);
}

Session::Session(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "type") {
                QString stype = xmlReader.readElementText();
                if (stype == "bike") {
                    setType(SessionType::BIKE);
                } else if (stype == "run") {
                    setType(SessionType::RUN);
                } else {
                    setType(SessionType::RUN);
                }
                xmlReader.readNext();
            } else if (xmlReader.name() == "duration") {
                Duration tmpDur(xmlReader);
                duration = tmpDur;
                xmlReader.readNext();
            } else if (xmlReader.name() == "distance") {
                qDebug() << "read distance";
                Distance tmpDist(xmlReader);
                distance = tmpDist;
                xmlReader.readNext();
            } else if (xmlReader.name() == "description") {
                qDebug() << "read description";
                description = xmlReader.readElementText();
                xmlReader.readNext();
            } else if (xmlReader.name() == "name") {
                name = xmlReader.readElementText();
                xmlReader.readNext();
            } else if (xmlReader.name() == "maproute") {
                qDebug() << "read maproute";
                mapRoute.readXML(xmlReader);
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

