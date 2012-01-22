#include "defaultSessionPainter.h"
#include "bikeSession.h"
#include "runSession.h"
#include "session.h"

DefaultSessionPainter::DefaultSessionPainter() : padding(5) {
}

QPointF DefaultSessionPainter::paint(const RunSession& arun, const QPointF &aoffset, QPainter &apainter) const {
    QPointF curPos = aoffset;

    QFont serifFont("Times", 10);
    QFontMetricsF fm(serifFont);
    int fontPixHeight = fm.height();
    apainter.setFont(serifFont);

    QString run = QString("Run: ");
    int runWidth = fm.width(run);
    QString txt = QString("Run: %1").arg(arun.getSession()->getDistance().toString());
    curPos += QPointF(0, fontPixHeight);
    apainter.drawText(curPos, txt);

    curPos += QPointF(runWidth, fontPixHeight + 2);
    QString runTime = QString("%1").arg(arun.getSession()->getDuration().toString());
    apainter.drawText(curPos, runTime);

    curPos -= QPointF(runWidth, 0);
    curPos += QPointF(0, padding);
    return curPos;
}

QPointF DefaultSessionPainter::paint(const BikeSession& abike, const QPointF &aoffset, QPainter &apainter) const {
    QPointF curPos = aoffset;

    QFont serifFont("Times", 10);
    QFontMetricsF fm(serifFont);
    int fontPixHeight = fm.height();
    apainter.setFont(serifFont);

    QString bike = QString("Bike: ");
    int bikeWidth = fm.width(bike);
    QString txt = QString("Bike: %1").arg(abike.getSession()->getDistance().toString());
    curPos += QPointF(0, fontPixHeight);
    apainter.drawText(curPos, txt);

    curPos += QPointF(bikeWidth, fontPixHeight+2);
    QString bikeTime = QString("%1").arg(abike.getSession()->getDuration().toString());
    apainter.drawText(curPos, bikeTime);

    curPos -= QPointF(bikeWidth, 0);
    curPos += QPointF(0, padding);
    return curPos;
}

//QPointF DefaultSessionPainter::paint(const DaySchedule& schedule, const QPointF &aoffset, QPainter &apainter) const {
//    QPointF curPos = aoffset;
//    const DaySchedule::SessionList sl = schedule.getSessions();
//    DaySchedule::constSessionIter citer = sl.constBegin();
//    for( ; citer != sl.end(); citer++) {
//        (*citer)->paint()
//    }
//    return curPos;
//}
