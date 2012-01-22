#ifndef TRAINDAYWIDGET_H
#define TRAINDAYWIDGET_H

#include <QSharedPointer>

#include "scheduleCellWidget.h"
#include "week.h"
#include "daySchedule.h"

class SessionPainter;
class QSize;

class TrainDayWidget : public ScheduleCellWidget
{
    Q_OBJECT

public:
    TrainDayWidget(QSharedPointer<DaySchedule> const daySdl, SessionPainter *const sp, Week w, QWidget *parent=0);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    void createConnections();

    QSharedPointer<DaySchedule> const daySchedule;
    SessionPainter *const spainter;
};

#endif // TRAINDAYWIDGET_H
