#ifndef MAPANDOPTIONWIDGET_H
#define MAPANDOPTIONWIDGET_H

#include <QWidget>

class MapWidget;
class MapOptionWidget;
class QVBoxLayout;

class MapAndOptionWidget : public QWidget
{
    Q_OBJECT

public:
    MapAndOptionWidget(MapWidget* mw, MapOptionWidget* mow, QWidget* parent = 0);
    void setOptionVisible(bool);

private slots:
    void hideOptionWidget();
    void geocodeAddress() const;

private:
    MapWidget* mapWidget;
    MapOptionWidget* mapOptionWidget;

    QVBoxLayout* layout;

    void createLayout();
    void createConnections();
};

#endif // MAPANDOPTIONWIDGET_H
