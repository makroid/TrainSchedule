#include "summaryWidget.h"
#include "sessionPainter.h"

#include <QPainter>
#include <QPointF>
#include <QFont>
#include <QPalette>
#include <QFontMetrics>

SummaryWidget::SummaryWidget(QSharedPointer<WeekSummary> ws, SessionPainter* const sp, Week w, QWidget* parent)
    : ScheduleCellWidget(w, parent),
      summary(ws),
      spainter(sp)
{
    createConnections();
}

void SummaryWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    QRect rect = painter.viewport();
    painter.setBrush(QBrush(Qt::gray));
    painter.drawRect(rect);

    QPointF curPos(5,5);

    QFont serifFont("Times", 9);
    QFontMetricsF fm(serifFont);
    int fontPixHeight = fm.height();

    // box at the right edge of widget, labeled with week number
    painter.save();
    QRect weekOfYearRect(width()-fontPixHeight-4, 0, fontPixHeight+4, height());
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(weekOfYearRect);
    painter.rotate(90);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QString sideLabel = QString("WEEK %1").arg(week.week());
    int sideLabelLen = fm.width(sideLabel);
    painter.drawText(0, -width(),height(), fontPixHeight+8,  Qt::AlignCenter, sideLabel);
    painter.restore();

//    QFont serifFont("Times", 10);
//    QFontMetricsF fm(serifFont);
//    int fontPixHeight = fm.height();

    for (int i=0; i<summary->getSessions().length(); i++) {
        curPos = summary->getSessions().value(i)->paint(*spainter, curPos, painter);
    }
    // no session this week => print "recomp"
    if (summary->getSessions().length() == 0) {
        QFont serifFont("Times", 14, QFont::Bold);
        QFontMetricsF fm(serifFont);
        fontPixHeight = fm.height();
        painter.setFont(serifFont);
        painter.setPen(QPen(Qt::red));

        curPos += QPointF(0, fontPixHeight);
        painter.drawText(curPos, "recomp");
        int space = sideLabelLen - curPos.y() + 20;

        curPos += QPointF(0, space);
    }

    plotHeight = curPos.y();

    emit plotHeightChanged(week);
}

void SummaryWidget::createConnections() {
    connect(summary.data(), SIGNAL(updated()), this, SLOT(repaint()));
}

QSize SummaryWidget::sizeHint() const {
    return QSize(100, 40);
}
