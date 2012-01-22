#ifndef MAPANDCONTROLWIDGET_H
#define MAPANDCONTROLWIDGET_H

#include "mapWidget.h"
#include "mapControlWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

class MapAndControlWidget : public QSplitter
{
    Q_OBJECT

public:
    MapAndControlWidget(QWidget* parent = 0);

    MapWidget* getMapWidget() const { return mapWidget; }
    MapControlWidget* getMapControlWidget() const { return mapControlWidget; }

private slots:
    void initializeMap();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    MapWidget* mapWidget;
    MapControlWidget* mapControlWidget;
};

#endif // MAPANDCONTROLWIDGET_H
