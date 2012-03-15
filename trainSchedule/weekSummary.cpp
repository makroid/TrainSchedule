#include "weekSummary.h"
#include <QtDebug>

WeekSummary::WeekSummary() : days(0) {
}

void WeekSummary::update() {
    // clear sessions list
    weekSessions.clear();
    typeToIdx.clear();
    // and build new one
    for (int i=0; i<days->count(); i++) {
        for (int j=0; j<days->value(i)->getNumSessions(); j++) {
            // get type of session
            int sessionType = days->value(i)->getSession(j)->getTypeCode();
            // test if this type is already in summary
            QSharedPointer<Session> session = sessionForType(sessionType);
            session->addDistance(days->value(i)->getSession(j)->getDistance());
            session->addDuration(days->value(i)->getSession(j)->getDuration());
        }
    }
    emit updated();
}

QSharedPointer<Session>& WeekSummary::sessionForType(int type) {
    int idx = typeToIdx.value(type, -1);
    // type already seen
    if (idx != -1) {
        return weekSessions[idx];
    } else {
        Duration dur;
        Distance dist;
        QSharedPointer<Session> se = QSharedPointer<Session>(new Session(type, dur, dist));
        weekSessions.push_back(se);
        typeToIdx[type] = weekSessions.size()-1;
        return weekSessions.last();
    }
}
