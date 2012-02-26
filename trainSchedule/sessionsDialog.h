#ifndef SESSIONSDIALOG_H
#define SESSIONSDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QSharedPointer>


#include "daySchedule.h"
#include "trainData.h"
#include "sessionsWidget.h"
#include "mapAndControlWidget.h"


class SessionsDialog : public QDialog
{
    Q_OBJECT

public:
    // TODO: replace DaySchedule* by QSharedPointer
    SessionsDialog(QSharedPointer<TrainingData> td, Week w, int day, QWidget* parent=0);

    bool isModified() const;

private:
    void createWidgets();
    void createLayout();

    QSharedPointer<TrainingData> trainingData;
    Week week;
    int day;

    SessionsWidget* sessionsWidget;
    MapAndControlWidget* mapAndControlWiget;
    QHBoxLayout* layout;
};

#endif // SESSIONSDIALOG_H
