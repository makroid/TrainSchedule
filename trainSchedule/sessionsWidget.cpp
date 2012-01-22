#include "sessionsWidget.h"
#include <QtDebug>

SessionsWidget::SessionsWidget(DaySchedule *ds, MapWidget *const mw, MapControlWidget *const mcw, QWidget *parent)
    : QTabWidget(parent)
{
    newSessionWidget = new NewSessionWidget(ds, mw, mcw, this);
    existingSessionsWidget = new ExistingSessionsWidget(ds, mw, mcw, this);
    addTab(newSessionWidget, "new");
    addTab(existingSessionsWidget, "sessions");

    // enable "new" tab if no saved session available
    // enable "sessions" tab otherwise
    if (ds->getNumSessions() == 0) {
        setCurrentWidget(newSessionWidget);
    } else {
        setCurrentWidget(existingSessionsWidget);
        // load first session
        existingSessionsWidget->loadSession(0);
        mcw->setEnabled(false);
    }

     this->setFixedWidth(sizeHint().width());

    createConnections();
}

void SessionsWidget::updateMapControl(int aidx) {
    // changed to "new" tab
    if (aidx==this->indexOf(newSessionWidget)) {
        newSessionWidget->completeReset();
        newSessionWidget->enableControlWidget();
    } else {    
        existingSessionsWidget->setSessionDisabled();
    }
}

void SessionsWidget::createConnections() {
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateMapControl(int)));
}

bool SessionsWidget::isModified() const {
    return existingSessionsWidget->isWindowModified() || newSessionWidget->isWindowModified();
}

void SessionsWidget::showExistingSession(int aidx) {
    if (currentIndex() != indexOf(existingSessionsWidget)) {
        setCurrentWidget(existingSessionsWidget);
    }
    existingSessionsWidget->loadSession(aidx);
}
