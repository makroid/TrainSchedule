#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWebView>
#include <QSize>

#include "mapPage.h"

class MapRoute;

class MapWidget : public QWebView
{
    Q_OBJECT
public:
    MapWidget(QWidget* parent = 0);
    ~MapWidget();

    QSize minimumSizeHint() const;
    MapPage* getMapPage() const { return mapPage; }
    double getDistanceInMeter() const;
    void fillMapRoute(MapRoute&) const;
    int loadMapRoute(const MapRoute&) const;
    bool extendMapRoute(const MapRoute&) const;
    bool isCurRouteNull() const { return mapPage->evalIsCurRouteNull(); }

private:
    MapPage* mapPage;
};

#endif // MAPWIDGET_H
