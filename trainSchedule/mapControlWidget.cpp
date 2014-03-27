#include "mapControlWidget.h"
#include "mapWidget.h"
#include "kmlExportDialog.h"
#include "routeMarkerExportDialog.h"
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
    runnerRadioButton = new QRadioButton(tr("&runner"));
    runnerRadioButton->setChecked(true);
    bikerRadioButton = new QRadioButton(tr("&biker"));
    modeBox = new QGroupBox(tr("mode"));
    routeMarkerBox = new QCheckBox("show route markers");

    routeMarkerExportButton = new QPushButton(tr("export markers..."));
    kmlExportButton = new QPushButton(tr("export kml..."));
}

void MapControlWidget::createLayout() {
    layout = new QVBoxLayout;
    layout->addWidget(newRouteButton);
    layout->addWidget(undoButton);
    layout->addWidget(deleteButton);

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(runnerRadioButton);
    box->addWidget(bikerRadioButton);
    box->addWidget(manualRadioButton);
    modeBox->setLayout(box);

    layout->addWidget(modeBox);
    routeMarkerBox->setChecked(true);
    layout->addWidget(routeMarkerBox);

    layout->addStretch();

    layout->addWidget(routeMarkerExportButton);
    layout->addWidget(kmlExportButton);
    setLayout(layout);
}

void MapControlWidget::createConnections() {
    connect(newRouteButton, SIGNAL(clicked()), this, SLOT(startNewRoute()));
    connect(undoButton, SIGNAL(clicked()), this, SLOT(undoLastPoint()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRoute()));
    connect(runnerRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeRunnerAutomatic(bool)));
    connect(bikerRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeBikerAutomatic(bool)));
    connect(manualRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeManual(bool)));
    connect(routeMarkerBox, SIGNAL(toggled(bool)), this, SLOT(setShowRouteMarkers(bool)));
    connect(routeMarkerExportButton, SIGNAL(clicked()), this, SLOT(showRouteMarkerExportDialog()));
    connect(kmlExportButton, SIGNAL(clicked()), this, SLOT(showExportRouteAsKmlDialog()));

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
        runnerRadioButton->setChecked(true);
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


void MapControlWidget::setClickModeRunnerAutomatic (bool isAutomatic) {
    if (isAutomatic) {
        mapPage->evalSetModus(MapRoute::runnerAutomatic);
    }
}

void MapControlWidget::setClickModeBikerAutomatic (bool isAutomatic) {
    if (isAutomatic) {
        mapPage->evalSetModus(MapRoute::bikerAutomatic);
    }
}

void MapControlWidget::setClickModeManual (bool isManual) {
    if (isManual) {
        mapPage->evalSetModus(MapRoute::manual);
    }
}

void MapControlWidget::setShowRouteMarkers(bool isShow) {
    mapPage->evalSetShowRouteMarkers(isShow);
}


void MapControlWidget::reset(bool moveToCenter) {
    currentRouteId = -1;
    newRouteButton->setText("start new route");
    mapPage->evalDeleteCurRoute();
    runnerRadioButton->setChecked(true);
    if (moveToCenter) mapPage->evalMoveToCenter();
}

void MapControlWidget::setRecording() {
    newRouteButton->setText("recording...");
    //runnerRadioButton->setChecked(true);
}

void MapControlWidget::forwardModeRadioButton() {
    if (runnerRadioButton->isChecked()) {
        bikerRadioButton->setChecked(true);
    } else if (bikerRadioButton->isChecked()) {
        manualRadioButton->setChecked(true);
    } else {
        runnerRadioButton->setChecked(true);
    }
}

void MapControlWidget::showRouteMarkerExportDialog() {
    RouteMarkerExportDialog markerDialog(mapWidget, this);
    markerDialog.exec();
}

void MapControlWidget::showExportRouteAsKmlDialog() {
    KmlExportDialog kmlDialog(mapWidget, this);
    kmlDialog.exec();
}
