#include "weekMap.h"
#include "trainScheduleWidget.h"
#include "helpers.h"
#include "WeekMapListener.h"
#include "configData.h"


#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>
#include <QtDebug>

WeekMap::WeekMap()
{
    createConnections();
}

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

bool WeekMap::save(const QString &fileName) {
    // save output in string
    bool doCompress = ConfigData::instance().doCompressSdlFile();
    QString result;
    QXmlStreamWriter xmlWriter(&result);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    writeXML(xmlWriter);
    xmlWriter.writeEndDocument();

    QFile file(fileName);
    // write string to file
    if (doCompress) {
        qDebug() << "writing compressed file";
        if ( ! file.open(QFile::WriteOnly)) {
            // TODO warn
            qDebug() << "Warning: can not write" << fileName;
            return false;
        }
        QByteArray ba = qCompress(result.toUtf8());
        QDataStream dataOut(&file);
        dataOut << ba;
        file.close();
    } else {
        qDebug() << "writing UNcompressed file";
        if ( ! file.open(QFile::WriteOnly)) {
            // TODO warn
            return false;
        }
        QTextStream txtStream(&file);
        txtStream << result;
        file.close();
    }
    return true;
}

bool WeekMap::readXML(const QString &fileName) {
    QFile file(fileName);
    if ( ! file.open(QFile::ReadOnly)) {
        // TODO: warn
        return false;
    }

    QString xmlString;

    if (ConfigData::instance().doCompressSdlFile()) {
        qDebug() << "reading from compressed file";
        QDataStream dataIn(&file);
        QByteArray compressedByteArray;
        dataIn >> compressedByteArray;
        QByteArray xmlByteArray = qUncompress(compressedByteArray);
        xmlString = QString::fromUtf8(xmlByteArray.data(), xmlByteArray.size());
    } else {
        qDebug() << "reading UNcompressed file";
        QTextStream txtIn(&file);
        xmlString = txtIn.readAll();
    }

    qDebug() << "xmlString=" << xmlString;
    QXmlStreamReader xmlReader(xmlString);

    xmlReader.readNext();
    while( ! xmlReader.atEnd()) {
        if (xmlReader.isEndElement()) {
            break;
        }
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "weekmap") {
                qDebug() << "read weekmap";
                xmlReader.readNext();
            } else if (xmlReader.name() == "week") {
                qDebug() << "read week";
                readWeekXML(xmlReader);
            }
            else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
    file.close();
    return true;
}

void WeekMap::readWeekXML(QXmlStreamReader& xmlReader) {
    Week aweek(xmlReader);
    qDebug() << "after week read";
    xmlReader.readNext();
    qDebug() << "after read next";
    qDebug() << "next=" << xmlReader.name();
    if (xmlReader.isStartElement()) { qDebug() << "start element"; }
    if (xmlReader.isStartElement()) {
        if (xmlReader.name() == "weekschedule") {
            qDebug() << "read weekschedule";
            xmlReader.readNext();
            qDebug() << "next=" << xmlReader.name();
            QSharedPointer<WeekSchedule> ws = QSharedPointer<WeekSchedule>(new WeekSchedule(xmlReader));
            wmap.insert(aweek, ws);
            WeekChange wc(aweek, WeekChange::WeekInserted);
            emit newWeekInserted(wc);
            xmlReader.readNext();
        } else {
            Helper::skipUnknownElements(xmlReader);
        }
    } else {
        qDebug() << "in else";
        xmlReader.readNext();
    }
}

void WeekMap::updateListeners(WeekChange wc) {
    for (int i=0; i<listeners.length(); i++) {
        listeners[i]->changeInWeek(wc);
    }
}
