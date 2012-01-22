#ifndef WEEKMAP_H
#define WEEKMAP_H


#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QXmlStreamWriter>

#include "week.h"
#include "weekChange.h"
#include "weekSchedule.h"
#include "weekSummary.h"

class WeekMapListener;

class WeekMap : public QObject
{
    Q_OBJECT

public:
    WeekMap();

    // getters
    QSharedPointer<DaySchedule> getDayInWeek(const Week&, int day);
    QSharedPointer<WeekSummary> getWeekSummary(const Week&);
    int getDayVectorSizeForWeek(const Week& w) { return wmap[w]->getDays().size(); }

    // workers
    bool insertNewWeek(Week);
    bool removeWeek(Week);

    void addWeekMapListener(WeekMapListener* listener);
    void removeWeekMapListener(WeekMapListener* listener);

    // readers/writers
    void writeXML(QXmlStreamWriter& writer) const;
    bool readXML(const QString& fileName);
    bool save(const QString& fileName);

signals:
    void newWeekInserted(WeekChange);
    void weekRemoved(WeekChange);

private slots:
    void updateListeners(WeekChange);

private:
    void createConnections();

    void readWeekXML(QXmlStreamReader&);

    typedef QMap<Week, QSharedPointer<WeekSchedule> >::const_iterator ConstWeekMapIter;
    QMap<Week, QSharedPointer<WeekSchedule> > wmap;
    QList<WeekMapListener*> listeners;
};

#endif // WEEKMAP_H
