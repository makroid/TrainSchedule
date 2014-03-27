#include "mapRoute.h"
#include "helpers.h"

#include <QtDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

// this is a pure data-storing class with mainly getters and setters

// mapId is indicator whether there is actually a route saved
MapRoute::MapRoute()
    : mapId(-1),
      distanceInMeter(0)
{}

MapRoute::~MapRoute() {}

void MapRoute::reverseRoute() {
    if (polyline.length() <= 1) return;
    for (int i=0; i<polyline.size(); i++) qDebug() << "polyline[" << i << "]=" << polyline[i].x() << "," << polyline[i].y();
    Helper::inplaceReverse(polyline);
    for (int i=0; i<polyline.size(); i++) qDebug() << "polyline[" << i << "]=" << polyline[i].x() << "," << polyline[i].y();
    Helper::inplaceReverse(distances);
    // reverse step history
    QList<int> newHistorySteps;
    for (int i=historySteps.size()-1; i>=0; i--) {
        newHistorySteps << (historySteps.last()+1) - historySteps[i];
    }
    historySteps = newHistorySteps;
    // update curPos and prevPos
    if (curPos != prevPos && polyline.size() >= 2) {
        prevPos = polyline[polyline.size()-2];
    } else {
        prevPos = polyline.last();
    }
    curPos = polyline.last();
}

QPointF MapRoute::getCurPos() const {
    return curPos;
}

QPointF MapRoute::getPrevPos() const {
    return prevPos;
}

QPointF MapRoute::getStartPos() const {
    if (polyline.size() > 0) {
        return polyline[0];
    }
    return QPointF(0,0);
}

QString MapRoute::getName() const {
    return name;
}

int MapRoute::getMapId() const {
    return mapId;
}

MapRoute::clickmodus MapRoute::getModus() const {
    return modus;
}

const QList<int>& MapRoute::getHistorySteps() const {
    return historySteps;
}

const QList<QPointF>& MapRoute::getPolyline() const {
    return polyline;
}

const QList<RouteMarker>& MapRoute::getRouteMarkers() const {
    return routeMarkers;
}

const QList<double>& MapRoute::getHistoryDists() const {
    return distances;
}

double MapRoute::getDistanceInMeter() const {
    return distanceInMeter;
}

void MapRoute::setCurPos(QPointF acurPos) {
    curPos = acurPos;
}

void MapRoute::setPrevPos(QPointF aprevPos) {
    prevPos = aprevPos;
}

void MapRoute::setName(QString aname) {
    name = aname;
}

void MapRoute::setModus(clickmodus amodus) {
    modus = amodus;
}

void MapRoute::setMapId(int aid) {
    mapId = aid;
}

void MapRoute::setDistanceInMeter(double adistanceInMeter) {
    distanceInMeter = adistanceInMeter;
}

void MapRoute::setHistorySteps(const QList<int>& ahistorySteps) {
    historySteps = ahistorySteps;
}

void MapRoute::setPolyline(const QList<QPointF> &apolyline) {
    polyline = apolyline;
}

void MapRoute::setRouteMarkers(const QList<RouteMarker> &markers) {
    routeMarkers = markers;
}

void MapRoute::setHistoryDists(const QList<double>& adistances) {
    distances = adistances;
}

QString MapRoute::toString() const {
    QString result = "";
    result += "curPos=(" + Helper::pointToQString(curPos) + ")\n";
    result += "prevPos=(" + Helper::pointToQString(prevPos) + ")\n";
    result += "name=" + name + "\n";
    result += QString("mapId=%1\n").arg(mapId);
    result += QString("distance=%1\n").arg(distanceInMeter);
    result += "historySteps=\n";
    for (int i=0; i<historySteps.length(); i++) {
        result += QString::number(historySteps[i]) + ", ";
    }
    result += "\n";
    result += "distances=\n";
    for (int i=0; i<distances.length(); i++) {
        result += QString::number(distances[i]) + ", ";
    }
    result += "\n";
    result += "polyline=\n";
    for (int i=0; i<polyline.length(); i++) {
        result += "(" + Helper::pointToQString(polyline[i]) + "), ";
    }
    result += "\n";
    return result;
}

void MapRoute::writeXml(QXmlStreamWriter &writer) const {
    writer.writeStartElement("maproute");
    writer.writeTextElement("curpos", Helper::pointToQString(curPos) );
    writer.writeTextElement("prevpos", Helper::pointToQString(prevPos) );
    writer.writeTextElement("name", name);
    writer.writeTextElement("mapid", QString::number(mapId));
    writer.writeTextElement("distanceinmeter", QString::number(distanceInMeter));
    writer.writeTextElement("modus", QString::number(modus));
    writer.writeStartElement("historysteps");
    for (int i=0; i<historySteps.length(); ++i) {
        writer.writeTextElement("step", QString::number(historySteps[i]));
    }
    writer.writeEndElement();
    writer.writeStartElement("distances");
    for (int i=0; i<distances.length(); ++i) {
        writer.writeTextElement("dist", QString::number(distances[i], 'f', 8));
    }
    writer.writeEndElement();
    writer.writeStartElement("polyline");
    for (int i=0; i<polyline.length(); ++i) {
        writer.writeTextElement("ll", Helper::pointToQString(polyline[i]));
    }
    writer.writeEndElement();
    writer.writeStartElement("routemarkers");
    for (int i=0; i<routeMarkers.length(); ++i) {
        writer.writeStartElement("marker");
        writer.writeTextElement("rank", QString::number(routeMarkers[i].getRank()));
        writer.writeTextElement("latlng", Helper::pointToQString(routeMarkers[i].getLatLng()));
        writer.writeTextElement("text", routeMarkers[i].getMarkerText());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndElement();
}

void MapRoute::readXML(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "curpos") {
                curPos = Helper::stringToPoint(xmlReader.readElementText());
                xmlReader.readNext(); // skips </curpos>
            } else if (xmlReader.name() == "prevpos") {
                prevPos = Helper::stringToPoint(xmlReader.readElementText());
                xmlReader.readNext(); // skips </prevpos>
            } else if (xmlReader.name() == "name") {
                name = xmlReader.readElementText();
                xmlReader.readNext(); // skips </name>
            } else if (xmlReader.name() == "mapid") {
                mapId = xmlReader.readElementText().toInt();
                xmlReader.readNext(); // skips </mapid>
            } else if (xmlReader.name() == "distanceinmeter") {
                distanceInMeter = xmlReader.readElementText().toDouble();
                xmlReader.readNext(); // skips </distanceinmeter>
            } else if (xmlReader.name() == "modus") {
                QString m = xmlReader.readElementText();
                if (m == "3") {
                    modus = manual;
                } else if (m == "2"){
                    modus = bikerAutomatic;
                } else {
                    modus = runnerAutomatic;
                }
                xmlReader.readNext();
            } else if (xmlReader.name() == "historysteps") {
                readHistoryStepsXML(xmlReader);
                xmlReader.readNext();
            } else if (xmlReader.name() == "distances") {
                readDistancesXML(xmlReader);
                xmlReader.readNext();
            } else if (xmlReader.name() == "polyline") {
                readPolylineXML(xmlReader);
                xmlReader.readNext();
            } else if (xmlReader.name() == "routemarkers") {
                readRouteMarkersXML(xmlReader);
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void MapRoute::readHistoryStepsXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "step") {
                historySteps.push_back(xmlReader.readElementText().toInt());
                xmlReader.readNext(); // skips </step>
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void MapRoute::readDistancesXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "dist") {
                distances.push_back(xmlReader.readElementText().toDouble());
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void MapRoute::readPolylineXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "ll") {
                polyline.push_back(Helper::stringToPoint(xmlReader.readElementText()));
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void MapRoute::readRouteMarkersXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "marker") {
                RouteMarker rm(xmlReader);
                routeMarkers.push_back(rm);
                xmlReader.readNext();
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}
