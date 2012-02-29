#include "myRoutesWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>

#include "myRoutesTable.h"
#include "mapWidget.h"
#include "mapControlWidget.h"
#include "mapAndControlWidget.h"
#include "mapRoute.h"
#include "myRoutesAddDialog.h"

MyRoutesWidget::MyRoutesWidget(QSharedPointer<TrainingData> td, MapAndControlWidget* const macw, QWidget* parent)
    : QWidget(parent),
      trainingData(td),
      mapWidget(macw->getMapWidget()),
      mapControlWidget(macw->getMapControlWidget())
{
    createWidgets();
    createLayout();
    createConnections();
}

void MyRoutesWidget::createWidgets() {
    myRoutesTableLabel = new QLabel(tr("My routes"));
    myRoutesTable = new MyRoutesTable(trainingData, mapWidget, this);
    addButton = new QPushButton(tr("Add..."));
    removeButton = new QPushButton(tr("Remove"));
    filterLabel = new QLabel(tr("Filter"));
    filterLineEdit = new QLineEdit;
}

void MyRoutesWidget::createLayout() {
    layout = new QGridLayout;
    layout->addWidget(myRoutesTableLabel, 0, 0);
    layout->addWidget(myRoutesTable, 1, 0, 1, 2);
    layout->addWidget(addButton, 2, 0);
    layout->addWidget(removeButton, 2, 1);

    setLayout(layout);
}

void MyRoutesWidget::createConnections() {
    connect(addButton, SIGNAL(clicked()), this, SLOT(addRouteToMyRoutes()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeRouteFromMyRoutes()));
    connect(myRoutesTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(insertMyRouteIntoMap(QModelIndex)));
}

void MyRoutesWidget::addRouteToMyRoutes() {
    if (mapWidget->isCurRouteNull()) return;

    MyRoutesAddDialog dialog;
    if (dialog.exec()) {
        QString routeName = dialog.nameLineEdit->text();
        MapRoute route;
        mapWidget->fillMapRoute(route);

        route.setName(routeName);
        myRoutesTable->addEntry(route);

        if (dialog.reverseCheckBox->isChecked()) {
            QString reverseRouteName = dialog.reverseNameLineEdit->text();
            route.reverseRoute();
            route.setName(reverseRouteName);
            myRoutesTable->addEntry(route);
        }
        mapControlWidget->reset(false);
        setWindowModified(true);
    }
}

void MyRoutesWidget::removeRouteFromMyRoutes() {
    // remove selected route
    myRoutesTable->removeEntry();
    setWindowModified(true);
}

void MyRoutesWidget::insertMyRouteIntoMap(QModelIndex index) {
    bool startNewRoute = myRoutesTable->insertMyRouteIntoMap(index);
    // check whether new route started or existing one continued
    if (startNewRoute) {
        mapControlWidget->setCurrentRouteId(1);
        mapControlWidget->setRecording();
    }
}
