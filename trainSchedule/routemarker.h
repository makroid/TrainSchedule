#ifndef ROUTEMARKER_H
#define ROUTEMARKER_H

#include <QPointF>
#include <QString>
#include <QXmlStreamReader>

class RouteMarker
{
public:
    RouteMarker(int rank, double lat, double lng, QString text);
    RouteMarker(int rank, QPointF latlng, QString text);
    RouteMarker(QXmlStreamReader& xmlReader);

    int getRank() const { return rank; }
    QPointF getLatLng() const { return latlng; }
    QString getMarkerText() const { return markerText; }

private:
    int rank;
    QPointF latlng;
    QString markerText;
};

#endif // ROUTEMARKER_H
