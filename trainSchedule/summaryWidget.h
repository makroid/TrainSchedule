#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QSize>

#include "scheduleCellWidget.h"
#include "weekSummary.h"
#include "week.h"

class SummaryWidget : public ScheduleCellWidget
{
    Q_OBJECT

public:
    SummaryWidget(QSharedPointer<WeekSummary> ws, SessionPainter* const sp, Week w, QWidget* parent=0);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *);

private:
    void createConnections();

    QSharedPointer<WeekSummary> summary;
    SessionPainter* const spainter;
};

#endif // SUMMARYWIDGET_H
