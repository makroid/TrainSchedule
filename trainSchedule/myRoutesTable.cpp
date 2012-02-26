#include "myRoutesTable.h"
#include "helpers.h"

#include <QHeaderView>
#include <QStringList>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QtDebug>

#include "distSortFilterProxyModel.h"
#include "myRoutesModel.h"
#include "mapRoute.h"
#include "mapWidget.h"

MyRoutesTable::MyRoutesTable(QSharedPointer<TrainingData> td, MapWidget* mw, QWidget *parent)
    : QTableView(parent),
      mapWidget(mw),
      model(td->getMyRoutesModel().data())
{
    proxyModel = new DistSortFilterProxyModel(this);
    qDebug() << "model number of lines=" << model->getMyRouteCount();
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(0);
    // javascript is now able to communicate with proxy model, see setCurPos in javascript-file
    //mapWidget->getMapPage()->mainFrame()->addToJavaScriptWindowObject("QTMyRoutesProxyModel", proxyModel);
    mapWidget->getMapPage()->mainFrame()->addToJavaScriptWindowObject("QTMyRoutesTable", this);

    this->setModel(proxyModel);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->hide();
    this->setSelectionMode(QAbstractItemView::SingleSelection);    
}

void MyRoutesTable::addEntry(const MapRoute &amapRoute) {
    // inserts empty MapRoute into model
    model->insertRows(0, 1, QModelIndex());
    QModelIndex index = model->index(0, 0, QModelIndex());
    model->setData(index, amapRoute.getName(), Qt::DisplayRole);
    model->setMyRoute(index, amapRoute);
}

void MyRoutesTable::editEntry() {

}

void MyRoutesTable::removeEntry() {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indices = selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indices) {
        int row = proxyModel->mapToSource(index).row();
        model->removeRows(row, 1, QModelIndex());
    }
}

int MyRoutesTable::insertMyRouteIntoMap(QModelIndex index) {
    int row = proxyModel->mapToSource(index).row();
    return mapWidget->extendMapRoute(model->getMyRoute(row));
}

void MyRoutesTable::setPosAndSort(double lat, double lng) {
    //if (this->isVisible() == false) return;
    proxyModel->setCurPos(lat, lng);
    this->sortByColumn(0, Qt::AscendingOrder);
}
