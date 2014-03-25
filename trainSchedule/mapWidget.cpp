#include "mapWidget.h"

#include <QSizePolicy>
#include <QTimer>
#include <QInputDialog>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>

// This is more or less a wrapper of MapPage

MapWidget::MapWidget(QWidget* parent) : QWebView(parent)
{
    mapPage = new MapPage(this);
    this->setPage(mapPage);

    createActions();

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

int MapWidget::loadMapRoute(const MapRoute &amapRoute) const {
    return mapPage->loadMapRoute(amapRoute);
}

bool MapWidget::extendMapRoute(const MapRoute &amapRoute) const {
    return mapPage->extendMapRoute(amapRoute);
}

void MapWidget::addMarker() {
    bool ok;
    QString markerText = QInputDialog::getText(this, tr("Set name for Marker"),
                                         tr("Marker:"), QLineEdit::Normal,
                                         tr(""), &ok);
    if (ok && !markerText.isEmpty()) {
        qDebug() << "input=" + markerText;
        mapPage->evalCreateAndAddRouteMarker(clickPos.x(), clickPos.y(), markerText);
    }
}

void MapWidget::createActions() {
    addMarkerAction = new QAction(tr("&Add Marker"), this);
    addMarkerAction->setShortcut(QKeySequence::New);
    connect(addMarkerAction, SIGNAL(triggered()), this, SLOT(addMarker()));
}

void MapWidget::mousePressEvent(QMouseEvent *event) {
    clickPos = event->pos();

    if (event->button() == Qt::RightButton) {
        showContextMenu(event->pos());
    }

    QWebView::mousePressEvent(event);
}

void MapWidget::showContextMenu(const QPoint& pos) {
    QPoint globalPos = this->mapToGlobal(pos);

    QMenu myMenu;
    addMarkerAction->setEnabled( ! mapPage->evalIsCurRouteNull());
    myMenu.addAction(addMarkerAction);

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem) {
    } else { }
}
