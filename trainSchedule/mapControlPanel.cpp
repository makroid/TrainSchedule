#include "mapControlPanel.h"
#include "mapRoutesTable.h"
#include "mapPage.h"
#include "mapWidget.h"

#include <QList>
#include <QString>
#include <QTimer>
#include <QHeaderView>
#include <QtDebug>

MapControlPanel::MapControlPanel(MapWidget * const amapWidget, QWidget *parent)
    : QWidget(parent),
      mapWidget(amapWidget),
      mapPage(amapWidget->getMapPage())
{
    createButtons();
    createLabels();
    createRouteTable();
    createLayout();
    currentRouteId = -1;
}

void MapControlPanel::createButtons() {
    newRouteButton = new QPushButton(tr("start new route"));
    undoButton = new QPushButton(tr("undo last step"));
    deleteButton = new QPushButton(tr("delete route"));
    takeRoutesButton = new QPushButton(tr("take route(s)"));

    manualRadioButton = new QRadioButton(tr("&manual"));
    automaticRadioButton = new QRadioButton(tr("&automatic"));
    automaticRadioButton->setChecked(true);
    modeBox = new QGroupBox(tr("mode"));

    // connections
    connect(newRouteButton, SIGNAL(clicked()), this, SLOT(startNewRoute()));
    connect(undoButton, SIGNAL(clicked()), this, SLOT(undoLastPoint()));
    connect(automaticRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeAutomatic(bool)));
    connect(manualRadioButton, SIGNAL(toggled(bool)), this, SLOT(setClickModeManual(bool)));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRoute()));
    connect(takeRoutesButton, SIGNAL(clicked()), this, SLOT(takeRoutes()));
}

void MapControlPanel::createLabels() {
    distanceLabel = new QLabel(tr("Distance"));
    concreteDistanceLabel = new QLabel(tr("0.0"));

    routesLabel = new QLabel(tr("Routes"));

    connect(mapWidget, SIGNAL(positionChanged()), this, SLOT(updateDistanceHelper()));
}

void MapControlPanel::createRouteTable() {
    routesTable = new MapRoutesTable(this);
    connect(routesTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(switchToRoute(int)));
}

void MapControlPanel::createLayout() {
    // mode radio buttons
    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(automaticRadioButton);
    box->addWidget(manualRadioButton);
    modeBox->setLayout(box);

    layout = new QVBoxLayout;

    int defaultVSpacing = layout->spacing();
    layout->addWidget(newRouteButton);
    layout->addWidget(undoButton);
    layout->addWidget(deleteButton);
    layout->setSpacing(defaultVSpacing+50);

    distanceLayout = new QHBoxLayout;
    distanceLayout->addWidget(distanceLabel);
    distanceLayout->addWidget(concreteDistanceLabel);

    layout->addLayout(distanceLayout);
    layout->addWidget(modeBox);
    layout->setSpacing(defaultVSpacing);
    layout->addWidget(routesLabel);
    layout->addWidget(routesTable);

    layout->addStretch();

    layout->addWidget(takeRoutesButton);

    setLayout(layout);
}

void MapControlPanel::startNewRoute() {
    int id = mapPage->evalStartNewRoute();
    currentRouteId = id;

    automaticRadioButton->setChecked(true);

    // update routes table
    QList<QString> newItem;
    newItem.push_back(QString::number(id));
    newItem.push_back(""); // no name yet
    newItem.push_back("run"); // TODO: chance this behaviour
    int row = routesTable->addNewItem(newItem);

    routesTable->selectRow(row);

    // update distance
    updateDistance(0.0);

}

void MapControlPanel::deleteRoute() {
    if (currentRouteId != -1) {
        bool success = mapPage->evalDeleteRoute(currentRouteId);

        if (success) {
            // update
            updateDistance(0.0);
            routesTable->removeItem(currentRouteId);

            currentRouteId = -1;
        }
    }
}

void MapControlPanel::undoLastPoint() {
    mapPage->evalUndo();
    // wait until undo() has performed its action
    QTimer::singleShot(500, this, SLOT(updateDistanceHelper()));
}

void MapControlPanel::setClickModeAutomatic (bool isAutomatic) {
    if (isAutomatic) {
        mapPage->evalSetModus(MapRoute::automatic);
    }
}

void MapControlPanel::setClickModeManual (bool isManual) {
    if (isManual) {
        mapPage->evalSetModus(MapRoute::manual);
    }
}

void MapControlPanel::switchToRoute(int arow) {
    int id = routesTable->item(arow, 0)->text().toInt();
    if (id == currentRouteId) { return; }

    bool success = mapPage->evalSwitchToRoute(id);
    if (success) {
        routesTable->selectRow(arow);
        currentRouteId = id;
        updateDistanceHelper();
        automaticRadioButton->setChecked(true);
        mapPage->evalSetModus(MapRoute::automatic);
    }
}

void MapControlPanel::takeRoutes() {

}

void MapControlPanel::updateDistance(double d) {
    concreteDistanceLabel->setText(QString::number(d));
}

void MapControlPanel::updateDistanceHelper() {
    double d = mapWidget->getDistanceInMeter();
    updateDistance(d);
}
