#ifndef WEEKSUMMARY_H
#define WEEKSUMMARY_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QList>
#include <QSharedPointer>

#include "distance.h"
#include "duration.h"
#include "daySchedule.h"

class WeekSchedule;

class WeekSummary : public QObject
{
    Q_OBJECT

public:
    typedef QVector<QSharedPointer<DaySchedule> > DayVector;
    typedef QList<QSharedPointer<Session> > SessionList;

    WeekSummary();

    void setDays(DayVector* adays) { days = adays; }
    const SessionList& getSessions() const { return weekSessions; }

public slots:
    void update();

signals:
    void updated();

private:
    QSharedPointer<Session>& sessionForType(int type);

    SessionList weekSessions;
    QMap<int, int> typeToIdx;
    DayVector* days;
};

#endif // WEEKSUMMARY_H
