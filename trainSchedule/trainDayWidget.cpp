#include "trainDayWidget.h"
#include "daySchedule.h"
#include "sessionPainter.h"
#include "sessionsDialog.h"
#include "week.h"
#include "weekMap.h"

#include <QPainter>
#include <QPointF>
#include <QSize>
#include <QtDebug>


TrainDayWidget::TrainDayWidget(QSharedPointer<TrainingData> td, SessionPainter *const sp, Week w, int d, QWidget *parent)
    : ScheduleCellWidget(w, parent),
      trainingData(td),
      daySchedule(td->getWeekMap()->getDayInWeek(w, d)),
      spainter(sp),
      week(w),
      day(d)
{
    createConnections();    
}

void TrainDayWidget::createConnections() {
    connect(daySchedule.data(), SIGNAL(newSessionAdded(int)), this, SLOT(repaint()));
    connect(daySchedule.data(), SIGNAL(sessionDeleted(int)), this, SLOT(repaint()));
    connect(daySchedule.data(), SIGNAL(sessionChanged(int)), this, SLOT(repaint()));
}

// create painter and delegate painting to daySchedule
// final height of plotted region is given to plotHeightChanged
// signal (together with row) if height has changed
// => signal is processed by TrainScheduleWidget
void TrainDayWidget::paintEvent(QPaintEvent * /*event*/) {

    QPointF curPos(5,5);
    QPainter painter(this);

    curPos = daySchedule->paint(*spainter, curPos, painter);
    plotHeight = curPos.y();

    emit plotHeightChanged(week);
}

void TrainDayWidget::mouseDoubleClickEvent(QMouseEvent *) {
    SessionsDialog sdialog(trainingData, week, day, this);
    sdialog.exec();
    setWindowModified(sdialog.isModified());
}

QSize TrainDayWidget::sizeHint() const {
    return QSize(100, 40);
}


