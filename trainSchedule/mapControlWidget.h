#ifndef MAPCONTROLWIDGET_H
#define MAPCONTROLWIDGET_H

#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>

class MapPage;
class MapWidget;
class KmlParameters;

class MapControlWidget : public QWidget
{
    Q_OBJECT

public:
    MapControlWidget(MapWidget * const mapWidget, QWidget *parent=0);

    QSize sizeHint() const { return minimumSizeHint(); }
    void reset(bool moveToCenter=true);
    void setRecording();
    void forwardModeRadioButton();

    int getCurrentRouteId() const { return currentRouteId; }
    void setCurrentRouteId(int crid) { currentRouteId = crid; }

private slots:
    void startNewRoute();
    void deleteRoute();
    void undoLastPoint();
    void setClickModeRunnerAutomatic(bool);
    void setClickModeBikerAutomatic(bool);
    void setClickModeManual(bool);
    void showExportRouteAsKmlDialog();
    void setShowRouteMarkers(bool);
    void showRouteMarkerExportDialog();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    QPushButton *newRouteButton;
    QPushButton *undoButton;
    QPushButton *deleteButton;
    QRadioButton *manualRadioButton;
    QRadioButton *runnerRadioButton;
    QRadioButton *bikerRadioButton;
    QGroupBox *modeBox;
    QCheckBox *routeMarkerBox;

    QPushButton* routeMarkerExportButton;
    QPushButton* kmlExportButton;

    QVBoxLayout* layout;

    // this is ugly, but we need mapwiget for double click signal
    // and distance label updating!
    MapWidget * const mapWidget;
    MapPage * const mapPage;

    int currentRouteId;
};

#endif // MAPCONTROLWIDGET_H
