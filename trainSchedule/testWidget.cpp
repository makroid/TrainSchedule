#include "testWidget.h"

TestWidget::TestWidget(DaySchedule* aday) : day(aday)
{
    startPos.setX(0);
    startPos.setY(0);
    dfsp = new DefaultSessionPainter();
}

void TestWidget::paintEvent(QPaintEvent */*event*/) {
    QPainter painter(this);
    day->paint(*dfsp, startPos, painter);
}
