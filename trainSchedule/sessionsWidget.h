#ifndef SESSIONSWIDGET_H
#define SESSIONSWIDGET_H

#include <QTabWidget>
#include <QPushButton>

#include "newSessionWidget.h"
#include "existingSessionsWidget.h"

class SessionsWidget : public QTabWidget
{
    Q_OBJECT

public:
    SessionsWidget(DaySchedule* ds, MapWidget* const mw, MapControlWidget* const mcw, QWidget* parent = 0);

    bool isModified() const;

    void showExistingSession(int idx);

private slots:
    void updateMapControl(int idx);


private:
    void createConnections();

    NewSessionWidget* newSessionWidget;
    ExistingSessionsWidget* existingSessionsWidget;
};

#endif // SESSIONSWIDGET_H
