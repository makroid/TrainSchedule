#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QtGui>

#include "defaultSessionPainter.h"
#include "daySchedule.h"

class TestWidget : public QWidget
{
public:
    TestWidget(DaySchedule* aday);

protected:
    void paintEvent(QPaintEvent *);

private:
     QPoint startPos;
     DefaultSessionPainter* dfsp;
     DaySchedule* day;
};

#endif // TESTWIDGET_H
