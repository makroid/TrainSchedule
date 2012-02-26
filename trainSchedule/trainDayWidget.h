#ifndef TRAINDAYWIDGET_H
#define TRAINDAYWIDGET_H

#include <QSharedPointer>

#include "scheduleCellWidget.h"
#include "trainData.h"
#include "daySchedule.h"

class SessionPainter;
class QSize;
class Week;

class TrainDayWidget : public ScheduleCellWidget
{
    Q_OBJECT

public:
    TrainDayWidget(QSharedPointer<TrainingData> td, SessionPainter *const sp, Week w, int day, QWidget *parent=0);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    void createConnections();

    QSharedPointer<TrainingData> trainingData;
    QSharedPointer<DaySchedule> const daySchedule;
    SessionPainter *const spainter;
    Week week;
    int day;
};

#endif // TRAINDAYWIDGET_H
