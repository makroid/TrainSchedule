#ifndef MAPSPAGE_H
#define MAPSPAGE_H

#include <QWebPage>
#include <QWebFrame>
#include <QVariant>
#include <QMessageBox>
#include <QtDebug>
#include <QList>
#include <QPointF>
#include <QString>

#include "mapRoute.h"

class MapPage : public QWebPage
{
    Q_OBJECT

public:
    MapPage(QWidget *parent = 0);

    void fillMapRoute(MapRoute&) const;
    void loadMapRoute(const MapRoute&) const;

    QString LatLng(const QPointF&) const;

public slots:
    // javascript function calls
    // getters
    bool evalIsCurRouteNull() const;
    double evalGetDistanceInMeter() const;
    QString evalGetName() const;
    QList<QPointF> evalGetPolyline() const;
    QList<int> evalGetHistorySteps() const;
    QList<double> evalGetHistoryDists()  const;
    int evalGetId() const;
    QPointF evalGetCurPos() const;
    QPointF evalGetPrevPos() const;

    // workers
    void evalUndo() const;
    int evalStartNewRoute() const;
    bool evalSwitchToRoute(int aid) const;
    bool evalDeleteCurRoute() const;
    void evalSetCenter() const;
    void evalMoveToCenter() const;

    // setters (these are const, because they dont
    // change this object)
    void evalSetDistanceInMeter(double distanceInMeter) const;
    void evalSetName(QString name) const;
    void evalSetPolyline(const QList<QPointF>&) const;
    void evalSetHistorySteps(const QList<int>&) const;
    void evalSetHistoryDists(const QList<double>&) const;
    void evalSetId(int) const;
    void evalSetCurPos(QPointF curPos) const;
    void evalSetPrevPos(QPointF prevPos) const;
    void evalSetModus(MapRoute::clickmodus) const;
    void evalSetDistanceMarkers() const;

protected:
    // this is needed to enable googlemaps for webkit
    QString userAgentForUrl ( const QUrl & url ) const;

private:
    void variantToPointF(const QVariant&, QPointF&) const;
    void variantToPointFList(const QVariant&, QList<QPointF>&) const;

};

#endif // MAPSPAGE_H
