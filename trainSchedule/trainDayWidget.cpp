#include "trainDayWidget.h"
#include "daySchedule.h"
#include "sessionPainter.h"
#include "sessionsDialog.h"

#include <QPainter>
#include <QPointF>
#include <QSize>
#include <QtDebug>

TrainDayWidget::TrainDayWidget(QSharedPointer<DaySchedule> const daySdl, SessionPainter *const sp, Week w, QWidget *parent)
    : ScheduleCellWidget(w, parent),
      daySchedule(daySdl),
      spainter(sp)
{
    qDebug() << daySchedule->toTestString();
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
    SessionsDialog sdialog(daySchedule.data(), this);
    sdialog.exec();
    setWindowModified(sdialog.isModified());
}

QSize TrainDayWidget::sizeHint() const {
    return QSize(100, 40);
}


