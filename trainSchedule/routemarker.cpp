#include "routemarker.h"
#include "helpers.h"

RouteMarker::RouteMarker(int rank, QPointF latlng, QString text)
{
    this->rank       = rank;
    this->latlng     = latlng;
    this->markerText = text;
}

RouteMarker::RouteMarker(int rank, double lat, double lng, QString text)
{
    this->rank       = rank;
    this->latlng     = QPointF(lat, lng);
    this->markerText = text;
}

RouteMarker::RouteMarker(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "rank") {
                rank = xmlReader.readElementText().toInt();
                xmlReader.readNext();
            } else if (xmlReader.name() == "latlng") {
                QString ll = xmlReader.readElementText();
                latlng = Helper::stringToPoint(ll);
                xmlReader.readNext();
            } else if (xmlReader.name() == "text") {
                markerText = xmlReader.readElementText();
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}
