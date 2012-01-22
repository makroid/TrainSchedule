#include "mapControlWidget.h"
#include "mapWidget.h"
#include "mapPage.h"

#include <QList>
#include <QString>
#include <QTimer>
#include <QtDebug>
#include <QSizePolicy>

MapControlWidget::MapControlWidget(MapWidget * const amapWidget, QWidget *parent)
    : QWidget(parent),
      mapWidget(amapWidget),
      mapPage(amapWidget->getMapPage())
{
    createWidgets();
    createLayout();
    createConnections();

    currentRouteId = -1;
}

void MapControlWidget::createWidgets() {
    newRouteButton = new QPushButton(tr("start new route"));
    undoButton = new QPushButton(tr("undo last step"));
    deleteButton = new QPushButton(tr("delete route"));

    manualRadioButton = new QRadioButton(tr("&manual"));
    automaticRadioButton = new QRadioButton(tr("&automatic"));
    automaticRadioButton->setChecked(true);
    modeBox = new QGroupBox(tr("mode"));
}

void MapControlWidget::createLayout() {
    layout = new QVBoxLayout;
    layout->addWidget(newRouteButton);
    layout->addWidget(undoButton);
    layout->addWidget(deleteButton);

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(automaticRadioButton);
    box->addWidget(manualRadioButton);
    modeBox->setLayout(box);

    layout->addWidget(modeBox);
    layout->addStretch();

    setLayout(layout);
}

void MapControlWidget::createConnections() {
    connect(newRouteButton, SIGNAL(clicked()), this, SLOT(startNewRoute()));
    connect(undoButton, SIGNAL(clicked()), this, SLOT(undoLastPoint()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRoute()));
    connect(automaticRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeAutomatic(bool)));
    connect(manualRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeManual(bool)));
}

void MapControlWidget::startNewRoute() {
    // avoid starting multiple routes
    // allow only one route here
    // -- evalStartNewRoute can start multiple routes in same map --
    qDebug() << "MapControlWidget::startNewRoute()";
    qDebug() << "currentRouteId=" << currentRouteId;
    if (currentRouteId == -1) {
        currentRouteId = mapPage->evalStartNewRoute();
        qDebug() << "currentRouteId after evalStartNewRoute=" << currentRouteId;
        automaticRadioButton->setChecked(true);
        newRouteButton->setText("recording...");
    }
    // update distance
}

void MapControlWidget::deleteRoute() {
    if (currentRouteId != -1) {
        // automatically sets distance to 0
        bool success = mapPage->evalDeleteCurRoute();
        if (success) {
            // update
            currentRouteId = -1;
            newRouteButton->setText("start new route");
        }
    }
}

void MapControlWidget::undoLastPoint() {
    // this automatically updates the distance
    mapPage->evalUndo();
}


void MapControlWidget::setClickModeAutomatic (bool isAutomatic) {
    if (isAutomatic) {
        mapPage->evalSetModus(MapRoute::automatic);
    }
}

void MapControlWidget::setClickModeManual (bool isManual) {
    if (isManual) {
        mapPage->evalSetModus(MapRoute::manual);
    }
}


void MapControlWidget::reset() {
    currentRouteId = -1;
    newRouteButton->setText("start new route");
    mapPage->evalDeleteCurRoute();
    automaticRadioButton->setChecked(true);
    mapPage->evalMoveToCenter();
}

void MapControlWidget::setRecording() {
    newRouteButton->setText("recording...");
    automaticRadioButton->setChecked(true);
}
