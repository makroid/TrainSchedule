#ifndef HELPERS_H
#define HELPERS_H

#include <cmath>

#include <QString>
#include <QStringList>
#include <QPointF>
#include <QPair>
#include <QXmlStreamReader>

namespace Helper {

    inline QString pointToQString(const QPointF& p) {
        return QString(QString::number(p.x(), 'f', 8) + "," + QString::number(p.y(), 'f', 8));
    }

    inline QPointF stringToPoint(const QString& str) {
        QStringList sl = str.split(",");
        return QPointF(sl[0].toDouble(), sl[1].toDouble());
    }

    inline void skipUnknownElements(QXmlStreamReader& xmlReader) {
        xmlReader.readNext();
        while(!xmlReader.atEnd()) {
            if (xmlReader.isEndElement()) {
                xmlReader.readNext();
                break;
            }
            if (xmlReader.isStartElement()) {
                skipUnknownElements(xmlReader);
            } else {
                xmlReader.readNext();
            }
        }
    }

    inline double distanceMeterBetween(const QPointF& p1, const QPointF& p2) {
        double R = 6378137; // distance in m
        double dLat = (p2.x() - p1.x()) * M_PI / 180;
        double dLng = (p2.y() - p1.y()) * M_PI / 180;
        double lat1 = p1.x() * M_PI / 180;
        double lat2 = p2.x() * M_PI / 180;
        double a = sin(dLat/2) * sin(dLat/2) + sin(dLng/2) * sin(dLng/2) * cos(lat1)*cos(lat2);
        double c = 2*atan2(sqrt(a), sqrt(1-a));
        double dist = R * c;
        return dist;
    }

    inline QPair<QPointF, QPointF> computeBound(const QList<QPointF>& path) {
        QPointF northWest;
        QPointF southEast;
        if (path.count() > 0) {
            northWest = path[0];
            southEast = path.last();
        } else {
            return QPair<QPointF,QPointF>(northWest, southEast);
        }
        for (int i=0; i<path.count(); i++) {
            if (path[i].x() < northWest.x()) northWest.setX(path[i].x());
            if (path[i].x() > southEast.x()) southEast.setX(path[i].x());
            if (path[i].y() < northWest.y()) northWest.setY(path[i].y());
            if (path[i].y() > southEast.y()) southEast.setY(path[i].y());
        }
        return QPair<QPointF,QPointF>(northWest, southEast);
    }

    template<typename T>
    inline void inplaceReverse(QList<T>& alist) {
        for(int k = 0; k < (alist.size()/2); k++) alist.swap(k, alist.size()-(k+1));
    }
}

#endif // HELPERS_H
