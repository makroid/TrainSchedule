#include "mapAndControlWidget.h"
#include "configData.h"

MapAndControlWidget::MapAndControlWidget(QWidget* parent) :
    QSplitter(parent)
{
    createWidgets();
    createLayout();
    createConnections();
}

void MapAndControlWidget::createWidgets() {
    mapWidget = new MapWidget(this);
    mapWidget->load(QUrl("mapsTest.html"));

    mapControlWidget = new MapControlWidget(mapWidget);
    mapControlWidget->setEnabled(false);
}

void MapAndControlWidget::createLayout() {
    addWidget(mapWidget);
    addWidget(mapControlWidget);

    QSizePolicy policy = sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    setSizePolicy(policy);
}

void MapAndControlWidget::createConnections() {
    connect(mapWidget, SIGNAL(loadFinished(bool)), mapControlWidget, SLOT(setEnabled(bool)));
    connect(mapWidget, SIGNAL(loadFinished(bool)), this, SLOT(initializeMap()));
}

void MapAndControlWidget::initializeMap() { 
    mapWidget->getMapPage()->mainFrame()->evaluateJavaScript("initialize();");
}
