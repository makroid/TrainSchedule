#ifndef NEWSESSIONTAB_H
#define NEWSESSIONTAB_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextEdit>

#include "durationSpinBox.h"
#include "mapWidget.h"
#include "mapControlWidget.h"


class SessionsWidget;
class DaySchedule;
class MapAndControlWidget;

class NewSessionWidget : public QWidget
{
    Q_OBJECT

public:
    NewSessionWidget(DaySchedule* ds, MapAndControlWidget* const macw, SessionsWidget* parent);
    void reset();
    void completeReset();
    void enableControlWidget();
    void disableControlWidget();

public slots:
    void updateDistance(double d);
    void saveSession();

private:
    DaySchedule* daySdl;
    // needed for distance update
    MapWidget* const mapWidget;
    MapControlWidget* const mapControlWidget;
    SessionsWidget* sessionsWidget;

    QLabel* typeLabel;
    QComboBox* typeCombo;
    QLabel* nameLabel;
    QLineEdit* nameLineEdit;
    QLabel* durationLabel;
    DurationSpinBox* durationSpinBox;
    QLabel* distanceLabel;
    QDoubleSpinBox* distanceSpinBox;
    QLabel* descriptionLabel;
    QTextEdit* descriptionEdit;
    QPushButton *mapButton;
    QPushButton* applyButton;

    QGridLayout* topLayout;
    QVBoxLayout* mainLayout;

    void createWidgets();
    void createLayout();
    void createConnections();
    bool validateSession() const;

};

#endif // NEWSESSIONTAB_H
