#ifndef MAPROUTESTABLE_H
#define MAPROUTESTABLE_H

#include <QTableWidget>
#include <QList>
#include <QString>

class MapRoutesTable : public QTableWidget
{
    Q_OBJECT
public:
    MapRoutesTable(QWidget *parent=0);

public slots:
    int addNewItem(const QList<QString>);
    int removeItem(int id);

private:
    QList<QString> columnNames;

};

#endif // MAPROUTESTABLE_H
