#include "trainScheduleWidget.h"
#include "week.h"
#include "sessionsDialog.h"
#include "trainData.h"

#include <QtDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QStringList>
#include <QSize>

TrainScheduleWidget::TrainScheduleWidget(QSharedPointer<TrainingData> td, SessionPainter* const sp, QWidget *parent)
    : QTableWidget(parent),
      trainingData(td),
      spainter(sp),
      widgetsPerRow(Week::daysPerWeek+1)
{
    setRowCount(0);
    setColumnCount(Week::daysPerWeek+1);
    verticalHeader()->setVisible(false);

    QStringList headerLabels;
    headerLabels << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun" << "Summary";
    setHorizontalHeaderLabels(headerLabels);
    resize(sizeHint());
    createConnections();
}

void TrainScheduleWidget::createConnections() {

}

void TrainScheduleWidget::changeInWeek(WeekChange wc) {
    if (wc.action == WeekChange::WeekInserted) {
        insertNewWeek(wc.week);
    } else if (wc.action == WeekChange::WeekRemoved) {
        removeWeek(wc.week);
    }
}

void TrainScheduleWidget::removeWeek(Week aweek) {
    int rowToRemove = 0;
    WeekWidgetMap::const_iterator citer = weekWidgets.find(aweek);
    if (citer == weekWidgets.end()) return;
    else {
        while(citer != weekWidgets.begin()) {
            rowToRemove++;
            citer--;
        }
    }
    // is this necessary? Cell item has ownership of widget
//    QVector<ScheduleCellWidget*>& tmp = weekWidgets[aweek];

//    for (int i=0; i<tmp.count(); i++) {
//        delete tmp[i];
//    }

    removeRow(rowToRemove);
    weekWidgets.remove(aweek);
}


void TrainScheduleWidget::insertNewWeek(Week aweek) {
    // find row where to insert new week
    int rowNewWeek = 0;
    QVector<ScheduleCellWidget*> tdws;
      // insert dummy - replace later
    WeekWidgetMap::const_iterator cwiter = weekWidgets.insert(aweek, tdws);
    while(cwiter != weekWidgets.constBegin()) {
        rowNewWeek++;
        cwiter--;
    }

    // create new daywigets
    // if daywidget changes plot-height, corresponding row must be adapted
    // => setRowHeightTo slot is called
    for (int day=0; day<Week::daysPerWeek; day++) {
        ScheduleCellWidget* tdw = new TrainDayWidget(trainingData, spainter, aweek, day, this);
        connect(tdw, SIGNAL(plotHeightChanged(Week)), this, SLOT(setRowHeightFor(Week)));
        tdws.push_back(tdw);
    }
    // create summary widget
    ScheduleCellWidget* summaryWidget = new SummaryWidget(trainingData->getWeekMap()->getWeekSummary(aweek), spainter, aweek, this);
    tdws.push_back(summaryWidget);
    connect(summaryWidget, SIGNAL(plotHeightChanged(Week)), this , SLOT(setRowHeightFor(Week)));

    // save widgets
    weekWidgets.insert(aweek, tdws);

    // insert new row and fill with widgets
    insertRow(rowNewWeek);
    for (int col=0; col<tdws.count(); col++) {
        setCellWidget(rowNewWeek, col, tdws[col]);        
    }
}

void TrainScheduleWidget::resizeEvent(QResizeEvent *) {
    int vscrollWidth = 0;
    if (verticalScrollBar()->isVisible()) {
        vscrollWidth += verticalScrollBar()->width();
    }
    // adjust column widths
    int newColWidth = (viewport()->width() - vscrollWidth - 1) / widgetsPerRow;

    int lastColWidth = viewport()->width() - (widgetsPerRow-1)*newColWidth ;
    for (int i=0; i<widgetsPerRow-1; i++) {
        setColumnWidth(i, newColWidth);
    }
    setColumnWidth(widgetsPerRow-1, lastColWidth);

    // to ensure that vertical scrollbar is updated (added or removed)
    updateGeometries();
}

QSize TrainScheduleWidget::sizeHint() const {
    return QSize(1000, 400);
}

void TrainScheduleWidget::setRowHeightFor(Week aweek) {
    // find row of week
    WeekWidgetMap::const_iterator witer = weekWidgets.find(aweek);
    int row = 0;
    while (witer != weekWidgets.constBegin()) {
        row++;
        witer--;
    }

    // find maximal height
    int maxHeight = 40;
    for (int i=0; i<weekWidgets.value(aweek).count(); i++) {
        int dwh = weekWidgets.value(aweek).at(i)->getPlotHeight();
        if (dwh > maxHeight) { maxHeight = dwh; }
    }
    setRowHeight(row, maxHeight);
}

bool TrainScheduleWidget::isModified() const {
    WeekWidgetMap::const_iterator cwiter = weekWidgets.constBegin();
    while(cwiter != weekWidgets.constEnd()) {
        const QVector<ScheduleCellWidget*>& ww = cwiter.value();
        for (int i=0; i<ww.size(); i++) {
            if (ww.value(i)->isWindowModified()) {
                return true;
            }
        }
        cwiter++;
    }
    return false;
}


void TrainScheduleWidget::setModified(bool b) {
    WeekWidgetMap::const_iterator cwiter = weekWidgets.constBegin();
    while(cwiter != weekWidgets.constEnd()) {
        const QVector<ScheduleCellWidget*>& ww = cwiter.value();
        for (int i=0; i<ww.size(); i++) {
            ww.value(i)->setWindowModified(b);
        }
        cwiter++;
    }
}
