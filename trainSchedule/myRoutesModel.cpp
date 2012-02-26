#include "myRoutesModel.h"
#include "helpers.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QVariant>
#include <QModelIndex>

#include <exception>

MyRoutesModel::MyRoutesModel(QObject* parent)
    : QAbstractTableModel(parent),
      maxColumns(1)
{
}

int MyRoutesModel::rowCount(const QModelIndex &index) const {
    return index.isValid() ? 0 : myRoutes.size();
}

int MyRoutesModel::columnCount(const QModelIndex &index) const {
    return index.isValid() ? 0 : maxColumns;
}

Qt::ItemFlags MyRoutesModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    return theFlags;
}

QVariant MyRoutesModel::data(const QModelIndex &index, int role) const {
    if ( ! index.isValid() || index.row() < 0 || index.row() >= myRoutes.count() ||
            index.column() < 0 || index.column() >= maxColumns) {
        return QVariant();
    }
    const MapRoute& item = myRoutes.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return item.getName();
        } else {
            Q_ASSERT(false);
        }
    } else if (role == Qt::UserRole) {
        return item.getStartPos();
    }
    return QVariant();
}

QVariant MyRoutesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    // column labels
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("name");
        default: Q_ASSERT(false);
        }
    }
    return QVariant(); // no row labels
}

bool MyRoutesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole || index.row() < 0 ||
            index.row() >= myRoutes.count() || index.column() < 0 || index.column() >= maxColumns) {
        return false;
    }
    MapRoute& item = myRoutes[index.row()];
    switch(index.column()) {
    case 0: {
        QString name = value.toString();
        if (name.isEmpty()) return false;
        item.setName(name);
    }
    default: Q_ASSERT(false);
    }
    emit dataChanged(index, index);
    return true;
}

bool MyRoutesModel::setMyRoute(const QModelIndex& index, const MapRoute& amapRoute) {
    if (!index.isValid() || index.row() < 0 || index.row() >= myRoutes.count()) {
        return false;
    }
    myRoutes[index.row()] = amapRoute;
    return true;
}

const MapRoute& MyRoutesModel::getMyRoute(int idx) const {
    if (idx < 0 || idx >= myRoutes.size()) {
        // TODO warn
    }
    return myRoutes[idx];
}

bool MyRoutesModel::insertRows(int row, int count, const QModelIndex &) {
    beginInsertRows(QModelIndex(), row, row + count -1);
    for (int i=0; i<count; i++) {
        myRoutes.insert(row, MapRoute());
    }
    endInsertRows();
    return true;
}

bool MyRoutesModel::removeRows(int row, int count, const QModelIndex &) {
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for (int i=0; i<count; i++) {
        myRoutes.removeAt(row);
    }
    endRemoveRows();
    return true;
}

void MyRoutesModel::readXML(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while( ! xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "maproute") {
                MapRoute route;
                route.readXML(xmlReader);
                insertRows(0, 1, QModelIndex());
                QModelIndex index = this->index(0, 0, QModelIndex());
                setData(index, route.getName(), Qt::DisplayRole);
                setMyRoute(index, route);
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void MyRoutesModel::writeXML(QXmlStreamWriter &writer) const {
    writer.writeStartElement("myroutes");
    for (int i=0; i<myRoutes.size(); i++) {
        myRoutes[i].writeXml(writer);
    }
    writer.writeEndElement();
}

