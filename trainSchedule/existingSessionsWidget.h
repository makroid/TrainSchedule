#ifndef EXISTINGSESSIONSWIDGET_H
#define EXISTINGSESSIONSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

#include "durationSpinBox.h"
#include "mapWidget.h"
#include "mapControlWidget.h"
#include "daySchedule.h"

class MapAndControlWidget;

class ExistingSessionsWidget : public QWidget
{
    Q_OBJECT

public:
    ExistingSessionsWidget(DaySchedule* ds, MapAndControlWidget* const mcaw, QWidget* parent = 0);

public slots:
    void updateDistance(double d);
    void reloadMap();
    void loadSession(int i);
    void deleteSession();
    void saveSession();
    void setSessionEnabled();
    void setSessionDisabled();
    void addItemToSessionCombo(int i);

    void updateSpeedTime(int sec);
    void updateSpeedDist(double KM);

private:
    //void showSession()

    DaySchedule* daySdl;
    // needed for distance update
    MapWidget* const mapWidget;
    MapControlWidget* const mapControlWidget;

    QLabel* sessionLabel;
    QComboBox* sessionCombo;
    QLabel* typeLabel;
    QComboBox* typeCombo;
    QLabel* nameLabel;
    QLineEdit* nameLineEdit;
    QLabel* durationLabel;
    DurationSpinBox* durationSpinBox;
    QLabel* distanceLabel;
    QDoubleSpinBox* distanceSpinBox;
    QLabel* speedLabel;
    QLineEdit* speedLineEdit;
    QLabel* descriptionLabel;
    QTextEdit* descriptionEdit;
    QPushButton* applyButton;
    QPushButton* editButton;
    QPushButton* deleteButton;

    QGridLayout* topLayout;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* mainLayout;

    void createWidgets();
    void createLayout();
    void createConnections();

    void setWidgetsEnabled(bool);

};

#endif // EXISTINGSESSIONSWIDGET_H
