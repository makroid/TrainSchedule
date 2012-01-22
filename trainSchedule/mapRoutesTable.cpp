#include "mapRoutesTable.h"

#include <QtDebug>

MapRoutesTable::MapRoutesTable(QWidget *aparent) : QTableWidget(aparent)
{
    columnNames.push_back("id");
    columnNames.push_back("name");
    columnNames.push_back("type");

    setColumnCount(columnNames.length());
    setRowCount(0);

    for (int i=0; i<columnNames.length(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(columnNames[i]);
        setHorizontalHeaderItem(i, item);
    }
    // id column
    setColumnWidth(0, 30);
}

int MapRoutesTable::addNewItem(const QList<QString> acontent) {
    int numRow = rowCount();
    insertRow(numRow);

    if (acontent.length() != columnCount()) {
        qDebug() << "Warning: MapRoutesTable::addNewItem: lenght differs";
    }
    for (int i=0; i<acontent.length(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setText(acontent[i]);
        if (i==0) {
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        }
        setItem(numRow, i, item);
    }
    return numRow;
}

int MapRoutesTable::removeItem(int aid) {
    int rowToRemove = -1;
    // find row by id
    for (int i=0; i<rowCount(); i++) {
        if (item(i, 0)->text().toInt() == aid) {
            rowToRemove = i;
            break;
        }
    }
    removeRow(rowToRemove);

    return rowToRemove;
}

