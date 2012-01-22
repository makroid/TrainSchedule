#ifndef MAPCONTROLWIDGET_H
#define MAPCONTROLWIDGET_H

#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MapPage;
class MapWidget;

class MapControlWidget : public QWidget
{
    Q_OBJECT

public:
    MapControlWidget(MapWidget * const mapWidget, QWidget *parent=0);

    QSize sizeHint() const { return minimumSizeHint(); }
    void reset();
    void setRecording();

    int getCurrentRouteId() const { return currentRouteId; }
    void setCurrentRouteId(int crid) { currentRouteId = crid; }

private slots:
    void startNewRoute();
    void deleteRoute();
    void undoLastPoint();
    void setClickModeAutomatic(bool);
    void setClickModeManual(bool);

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    QPushButton *newRouteButton;
    QPushButton *undoButton;
    QPushButton *deleteButton;
    QRadioButton *manualRadioButton;
    QRadioButton *automaticRadioButton;
    QGroupBox *modeBox;

    QVBoxLayout* layout;

    // this is ugly, but we need mapwiget for double click signal
    // and distance label updating!
    MapWidget * const mapWidget;
    MapPage * const mapPage;

    int currentRouteId;
};

#endif // MAPCONTROLWIDGET_H
