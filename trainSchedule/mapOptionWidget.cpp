#include "mapOptionWidget.h"
#include "mapWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

MapOptionWidget::MapOptionWidget(MapWidget* mw, QWidget* parent)
    : QWidget(parent),
      mapWidget(mw)
{
    createWidgets();
    createLayout();
}

void MapOptionWidget::createWidgets() {
    locationLabel = new QLabel("Location");
    locationLineEdit = new QLineEdit;
    closeButton = new QPushButton("close");
}

void MapOptionWidget::createLayout() {
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(locationLabel);
    layout->addWidget(locationLineEdit);
    layout->addWidget(closeButton, 0, Qt::AlignRight);
    setLayout(layout);
}

