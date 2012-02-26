#include "weekMap.h"
#include "trainScheduleWidget.h"
#include "helpers.h"
#include "WeekMapListener.h"
#include "configData.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtDebug>

WeekMap::WeekMap()
{
    createConnections();
}

// TODO: remove these connections => just use listener mechanism
void WeekMap::createConnections() {
    connect(this, SIGNAL(newWeekInserted(WeekChange)), this, SLOT(updateListeners(WeekChange)));
    connect(this, SIGNAL(weekRemoved(WeekChange)), this, SLOT(updateListeners(WeekChange)));
}

bool WeekMap::insertNewWeek(Week aweek) {
    if (wmap.contains(aweek)) {
        return false;
    }
    QSharedPointer <WeekSchedule> newWeek = QSharedPointer<WeekSchedule>(new WeekSchedule(aweek));
    wmap.insert(aweek, newWeek);

    WeekChange wc(aweek, WeekChange::WeekInserted);
    emit newWeekInserted(wc);
    return true;
}

bool WeekMap::removeWeek(Week aweek) {
    if ( ! wmap.contains(aweek)) return false;
    wmap.remove(aweek);

    WeekChange wc(aweek, WeekChange::WeekRemoved);
    emit weekRemoved(wc);
    return true;
}

QSharedPointer<DaySchedule> WeekMap::getDayInWeek(const Week& aweek, int aday) {
    QSharedPointer<WeekSchedule> w = wmap[aweek];
    return w->getDay(aday);
}

void WeekMap::addWeekMapListener(WeekMapListener* listener) {
    listeners.push_back(listener);
}

void WeekMap::removeWeekMapListener(WeekMapListener *listener) {
    listeners.removeAll(listener);
}

QSharedPointer<WeekSummary> WeekMap::getWeekSummary(const Week &aweek) {
    QSharedPointer<WeekSchedule> w = wmap[aweek];
    return w->getWeekSummary();
}

void WeekMap::writeXML(QXmlStreamWriter& writer) const {
    writer.writeStartElement("weekmap");
    ConstWeekMapIter i = wmap.constBegin();
    for ( ; i != wmap.constEnd(); i++)  {
        writer.writeTextElement("week", i.key().toString());
        i.value()->writeXML(writer);
    }
    writer.writeEndElement();
}

void WeekMap::readXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while( ! xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            xmlReader.readNext();
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "weekmap") {
                xmlReader.readNext();
            } else if (xmlReader.name() == "week") {
                readWeekXML(xmlReader);
            }
            else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}

void WeekMap::readWeekXML(QXmlStreamReader& xmlReader) {
    Week aweek(xmlReader);
    xmlReader.readNext();

    if (xmlReader.isStartElement()) {
        if (xmlReader.name() == "weekschedule") {
            xmlReader.readNext();          

            QSharedPointer<WeekSchedule> ws = QSharedPointer<WeekSchedule>(new WeekSchedule(xmlReader));
            wmap.insert(aweek, ws);
            WeekChange wc(aweek, WeekChange::WeekInserted);
            emit newWeekInserted(wc);
        } else {
            Helper::skipUnknownElements(xmlReader);
        }
    } else {
        xmlReader.readNext();
    }
}

void WeekMap::updateListeners(WeekChange wc) {
    for (int i=0; i<listeners.length(); i++) {
        listeners[i]->changeInWeek(wc);
    }
}
