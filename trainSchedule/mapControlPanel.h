#ifndef MAPCONTROLPANEL_H
#define MAPCONTROLPANEL_H

#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MapRoutesTable;
class MapPage;
class MapWidget;

class MapControlPanel : public QWidget
{
    Q_OBJECT

public:
    MapControlPanel(MapWidget * const mapWidget, QWidget *parent=0);

private slots:
    void startNewRoute();
    void deleteRoute();
    void updateDistance(double d);
    void updateDistanceHelper();
    void undoLastPoint();
    void setClickModeAutomatic(bool);
    void setClickModeManual(bool);
    void switchToRoute(int);
    void takeRoutes();

private:
    void createButtons();
    void createLabels();
    void createRouteTable();
    void createLayout();

    QPushButton *newRouteButton;
    QPushButton *undoButton;
    QPushButton *deleteButton;
    QPushButton *takeRoutesButton;
    QLabel *distanceLabel;
    QLabel *concreteDistanceLabel;
    QRadioButton *manualRadioButton;
    QRadioButton *automaticRadioButton;
    QGroupBox *modeBox;
    QLabel *routesLabel;
    MapRoutesTable *routesTable;

    QVBoxLayout *layout;
    QHBoxLayout *distanceLayout;

    // this is ugly, but we need mapwiget for double click signal
    // and distance label updating!
    MapWidget * const mapWidget;
    MapPage * const mapPage;

    int currentRouteId;
};

#endif // MAPCONTROLPANEL_H

