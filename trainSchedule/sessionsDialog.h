#ifndef SESSIONSDIALOG_H
#define SESSIONSDIALOG_H

#include <QDialog>
#include <QHBoxLayout>

#include "daySchedule.h"
#include "sessionsWidget.h"
#include "mapAndControlWidget.h"


class SessionsDialog : public QDialog
{
    Q_OBJECT

public:
    // TODO: replace DaySchedule* by QSharedPointer
    SessionsDialog(DaySchedule* ds, QWidget* parent=0);

    bool isModified() const;

private:
    void createWidgets();
    void createLayout();

    DaySchedule* daySdl;
    SessionsWidget* sessionsWidget;
    MapAndControlWidget* mapAndControlWiget;
    QHBoxLayout* layout;
};

#endif // SESSIONSDIALOG_H
