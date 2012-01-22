#include "mapWidget.h"

#include <QSizePolicy>
#include <QTimer>

// This is more or less a wrapper of MapPage

MapWidget::MapWidget(QWidget* parent) : QWebView(parent)
{
    mapPage = new MapPage(this);
    this->setPage(mapPage);

    QSizePolicy policy = this->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    this->setSizePolicy(policy);
}

MapWidget::~MapWidget() {
    delete mapPage;
}

QSize MapWidget::minimumSizeHint() const {
    return QSize(500, 400);
}

double MapWidget::getDistanceInMeter() const {
    return mapPage->evalGetDistanceInMeter();
}

// forwarding methods
void MapWidget::fillMapRoute(MapRoute &amapRoute) const {
    mapPage->fillMapRoute(amapRoute);
}

void MapWidget::loadMapRoute(const MapRoute &amapRoute) const {
    mapPage->loadMapRoute(amapRoute);
}
