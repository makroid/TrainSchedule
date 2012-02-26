#ifndef DISTSORTFILTERPROXYMODEL_H
#define DISTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointF>

class DistSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DistSortFilterProxyModel(QObject* parent=0);

    QPointF getCurPos() const { return curPos; }

public slots:
    void setCurPos(double lat, double lng);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QPointF curPos;
};

#endif // DISTSORTFILTERPROXYMODEL_H
