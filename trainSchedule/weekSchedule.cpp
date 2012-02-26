#include "weekSchedule.h"
#include "helpers.h"

#include <QtDebug>

WeekSchedule::WeekSchedule(Week aweek) : week(aweek) {
    QVector<QDate> dayDates = getDatesForWeek(aweek);
    for (int i=0; i<Week::daysPerWeek; i++) {
        DaySchedule* ds = new DaySchedule(dayDates[i]);
        days.push_back(QSharedPointer<DaySchedule>(ds));
        // TODO: move this to createConnections
//        connect(ds, SIGNAL(newSessionAdded(int)), this, SIGNAL(updated(int)));
//        connect(ds, SIGNAL(sessionDeleted(int)), this, SIGNAL(updated(int)));
//        connect(ds, SIGNAL(sessionChanged(int)), this, SIGNAL(updated(int)));
    }
    summary = QSharedPointer<WeekSummary>(new WeekSummary());
    summary->setDays(&days);
    createConnections();
}

WeekSchedule::WeekSchedule(QXmlStreamReader &xmlReader) {
    xmlReader.readNext();
    while(!xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "week") {
                Week w(xmlReader);
                week = w;
            } else if (xmlReader.name() == "dayschedule") {
                DaySchedule* ds = new DaySchedule(xmlReader);
                days.push_back(QSharedPointer<DaySchedule>(ds));
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
    summary = QSharedPointer<WeekSummary>(new WeekSummary());
    summary->setDays(&days);
    summary->update();
    createConnections();
}

QVector<QDate> WeekSchedule::getDatesForWeek(const Week& aweek) const {
    QDate newYear(aweek.year(), 1, 1);
    QVector<QDate> datesForWeek;

    /* find first day in first week, fdfw (in ayear)
      - start from 1.1.year => may be in week 1,52 or 53
      - if 52 or 53, complete this week until Sunday
      - if 1, go back to Monday
      go foward (week-1)*7 days
      the next 7 days are the days looked for
    */
    QDate firstDayFirstWeek;
    int numWeek = newYear.weekNumber();  // 1, 52 or 53
    if (numWeek == 52 || numWeek == 53) {
        firstDayFirstWeek = newYear.addDays(8 - newYear.dayOfWeek());
    } else { // if (numWeek == 1)
        firstDayFirstWeek = newYear.addDays( (-1)*(newYear.dayOfWeek() - 1) );
    }
    int daysBefore = Week::daysPerWeek * (aweek.week()-1);

    for (int i=0; i<Week::daysPerWeek; i++) {
        QDate dfw = firstDayFirstWeek;
        dfw = dfw.addDays(daysBefore+i);
        datesForWeek.push_back(dfw);
        qDebug() << "getDatesForWeek: " << dfw.toString("dd.MM.yyyy");
    }
    return datesForWeek;
}

void WeekSchedule::createConnections() {
    for (int i=0; i<days.count(); i++) {
        connect(days.at(i).data(), SIGNAL(newSessionAdded(int)), this, SIGNAL(updated(int)));
        connect(days.at(i).data(), SIGNAL(sessionDeleted(int)), this, SIGNAL(updated(int)));
        connect(days.at(i).data(), SIGNAL(sessionChanged(int)), this, SIGNAL(updated(int)));
    }
    connect(this, SIGNAL(updated(int)), summary.data(), SLOT(update()));
}

void WeekSchedule::writeXML(QXmlStreamWriter &writer) const {
    writer.writeStartElement("weekschedule");
    writer.writeTextElement("week", week.toString());
    for (int i=0; i<days.count(); ++i) {
        days.value(i)->writeXml(writer);
    }
    writer.writeEndElement();
}
