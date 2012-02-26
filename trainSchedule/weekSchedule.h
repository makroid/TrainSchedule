#ifndef WEEKSCHEDULE_H
#define WEEKSCHEDULE_H

#include <QObject>
#include <QVector>
#include <QDate>
#include <QSharedPointer>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "daySchedule.h"
#include "week.h"
#include "weekSummary.h"

class WeekSchedule : public QObject
{
    Q_OBJECT

public:
    WeekSchedule() {}
    WeekSchedule(Week week);
    WeekSchedule(QXmlStreamReader&);

    Week getWeek() const { return week; }

    typedef QVector<QSharedPointer<DaySchedule> > DayVector;

    const DayVector& getDays() const { return days; }
    QSharedPointer<WeekSummary> getWeekSummary() { return summary; }
    QSharedPointer<DaySchedule> getDay(int i) const { return days[i]; }

    QVector<QDate> getDatesForWeek(const Week& week) const;

    void writeXML(QXmlStreamWriter& writer) const;

signals:
    void updated(int);

private:
    void createConnections();

    Week week;
    DayVector days;
    QSharedPointer<WeekSummary> summary;
};

#endif // WEEKSCHEDULE_H
