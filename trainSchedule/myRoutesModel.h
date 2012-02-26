#ifndef MYROUTES_H
#define MYROUTES_H

#include <QAbstractTableModel>
#include <QList>

#include "mapRoute.h"

class QXmlStreamReader;
class QXmlStreamWriter;
class QVariant;
class QModelIndex;

class MyRoutesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MyRoutesModel(QObject* parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setMyRoute(const QModelIndex& index, const MapRoute&);
    const MapRoute& getMyRoute(int idx) const;
    int getMyRouteCount() const { return myRoutes.count(); }

    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    void readXML(QXmlStreamReader&);
    void writeXML(QXmlStreamWriter&) const;

private:
    QList<MapRoute> myRoutes;
    const int maxColumns;
};

#endif // MYROUTES_H
