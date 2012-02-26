#ifndef MYROUTESTABLE_H
#define MYROUTESTABLE_H

#include <QTableView>
#include <QSharedPointer>

#include "trainData.h"

class MyRoutesModel;
class MapWidget;
class DistSortFilterProxyModel;
class QModelIndex;
class MapRoute;

class MyRoutesTable : public QTableView
{
    Q_OBJECT

public:
    MyRoutesTable(QSharedPointer<TrainingData> td, MapWidget* const mw, QWidget* parent=0);

public slots:
    void addEntry(const MapRoute&);
    void editEntry();
    void removeEntry();
    int insertMyRouteIntoMap(QModelIndex);
    void setPosAndSort(double lat, double lng);

private:
    MapWidget* const mapWidget;

    MyRoutesModel* model;
    DistSortFilterProxyModel *proxyModel;

};

#endif // MYROUTESTABLE_H
