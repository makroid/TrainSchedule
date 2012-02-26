#ifndef TRAINSCHEDULEWIDGET_H
#define TRAINSCHEDULEWIDGET_H

#include <QTableWidget>
#include <QVector>
#include <QMap>
#include <QSharedPointer>
#include <QSize>

#include "weekMap.h"
#include "trainDayWidget.h"
#include "summaryWidget.h"
#include "sessionPainter.h"
#include "week.h"
#include "weekChange.h"
#include "WeekMapListener.h"
#include "scheduleCellWidget.h"


class QResizeEvent;
class TrainingData;

class TrainScheduleWidget : public QTableWidget, public WeekMapListener
{
    Q_OBJECT

public:
    TrainScheduleWidget(QSharedPointer<TrainingData> td, SessionPainter* const sp, QWidget* parent=0);

    QSize sizeHint() const;
    bool isModified() const;
    void setModified(bool b);

public slots:
    virtual void changeInWeek(WeekChange);  // Listener operation
    void setRowHeightFor(Week week);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void createConnections();
    void insertNewWeek(Week);
    void removeWeek(Week);

    const QSharedPointer<TrainingData> trainingData;
    SessionPainter* const spainter;

    // a week has 7 trainday widgets and one summary widget
    typedef QMap<Week, QVector<ScheduleCellWidget*> > WeekWidgetMap;
    WeekWidgetMap weekWidgets;

    int widgetsPerRow;
};

#endif // TRAINSCHEDULEWIDGET_H
