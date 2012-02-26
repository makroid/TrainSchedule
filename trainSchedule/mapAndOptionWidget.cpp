#include "mapAndOptionWidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QtDebug>

#include "mapWidget.h"
#include "mapOptionWidget.h"

MapAndOptionWidget::MapAndOptionWidget(MapWidget* mw, MapOptionWidget* mow, QWidget* parent)
    : QWidget(parent),
      mapWidget(mw),
      mapOptionWidget(mow)
{
    createLayout();
    setOptionVisible(false);
    createConnections();
}

void MapAndOptionWidget::createLayout() {
    layout = new QVBoxLayout;
    layout->addWidget(mapWidget);
    layout->addWidget(mapOptionWidget);

    setLayout(layout);
}

void MapAndOptionWidget::createConnections() {
    connect(mapOptionWidget->closeButton, SIGNAL(clicked()), this, SLOT(hideOptionWidget()));
    connect(mapOptionWidget->locationLineEdit, SIGNAL(returnPressed()), this, SLOT(geocodeAddress()));
}

void MapAndOptionWidget::setOptionVisible(bool b) {
    qDebug() << "setOptionsVisible=" << b;
    mapOptionWidget->setVisible(b);
    if (b) {
        mapOptionWidget->locationLineEdit->setFocus();
    }
}

void MapAndOptionWidget::hideOptionWidget() {
    qDebug() << "calling hideOptionWidget();";
    setOptionVisible(false);
}

void MapAndOptionWidget::geocodeAddress() const {
    mapWidget->getMapPage()->evalGeocodeAddress(mapOptionWidget->locationLineEdit->text());
}
