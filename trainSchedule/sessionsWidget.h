#ifndef SESSIONSWIDGET_H
#define SESSIONSWIDGET_H

#include <QTabWidget>
#include <QPushButton>
#include <QSharedPointer>

#include "trainData.h"

class MapAndControlWidget;
class MapControlWidget;
class Week;
class NewSessionWidget;
class ExistingSessionsWidget;
class MyRoutesWidget;

class SessionsWidget : public QTabWidget
{
    Q_OBJECT

public:
    SessionsWidget(QSharedPointer<TrainingData> td, MapAndControlWidget* const macw, Week w, int day, QWidget* parent = 0);

    bool isModified() const;

    void showExistingSession(int idx);

private slots:
    void updateMapControl(int idx);


private:
    void createConnections();

    NewSessionWidget* newSessionWidget;
    ExistingSessionsWidget* existingSessionsWidget;
    MyRoutesWidget* myRoutesWidget;

    MapControlWidget* mcw;
    int previousActiveWidget;
};

#endif // SESSIONSWIDGET_H
