#ifndef DAYSCHEDULE_H
#define DAYSCHEDULE_H

#include <QObject>
#include <QList>
#include <QDate>
#include <QString>
#include <QSharedPointer>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "session.h"
#include "sessionPainter.h"

class QPointF;
class QPainter;

class DaySchedule : public QObject
{
    Q_OBJECT

public:
    DaySchedule() {}
    DaySchedule(QDate adate);
    DaySchedule(const DaySchedule&);
    DaySchedule(QXmlStreamReader&);
    ~DaySchedule() {}

    // typedefs
    typedef QList<QSharedPointer<Session> > SessionList;
    typedef QList<QSharedPointer<Session> >::const_iterator constSessionIter;

    // getters
    const SessionList& getSessions() const { return sessions; }
    QSharedPointer<Session>& getSession(int i) { return sessions[i]; }
    int getNumSessions() const { return sessions.size(); }
    const QDate& getDate() const { return date; }

    // painter
    QPointF paint(const SessionPainter& spainter, const QPointF& offset, QPainter& painter) const;

    // workers
    void addSession(QSharedPointer<Session> asession);
    bool deleteSession(int idx);
    void update();

    // writer
    QString toTestString() const;
    void writeXml(QXmlStreamWriter& writer);

signals:
    // parameter is idx in sessions list
    void newSessionAdded(int);
    void sessionDeleted(int);
    void sessionChanged(int);

private:
    void readSessionsXML(QXmlStreamReader&);

    SessionList sessions;
    QDate date;
};

#endif // DAYSCHEDULE_H
