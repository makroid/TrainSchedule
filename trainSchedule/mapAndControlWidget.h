#ifndef MAPANDCONTROLWIDGET_H
#define MAPANDCONTROLWIDGET_H

#include <QWidget>
#include <QSplitter>

class MapWidget;
class MapControlWidget;
class MapOptionWidget;
class MapAndOptionWidget;
class QKeyEvent;

class MapAndControlWidget : public QSplitter
{
    Q_OBJECT

public:
    MapAndControlWidget(QWidget* parent = 0);

    MapWidget* getMapWidget() const { return mapWidget; }
    MapControlWidget* getMapControlWidget() const { return mapControlWidget; }

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void initializeMap();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    MapWidget* mapWidget;
    MapControlWidget* mapControlWidget;
    MapOptionWidget* mapOptionWidget;

    MapAndOptionWidget* mapAndOptionWidget;
};

#endif // MAPANDCONTROLWIDGET_H
