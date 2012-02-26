#include "sessionsWidget.h"
#include "mapAndControlWidget.h"
#include "mapControlWidget.h"
//#include "mapControlPanel.h"
#include "newSessionWidget.h"
#include "existingSessionsWidget.h"
#include "myRoutesWidget.h"
#include "week.h"

#include <QtDebug>


SessionsWidget::SessionsWidget(QSharedPointer<TrainingData> td, MapAndControlWidget* const macw, Week w, int d, QWidget *parent)
    : QTabWidget(parent),
      mcw(macw->getMapControlWidget())
{
    newSessionWidget = new NewSessionWidget(td->getWeekMap()->getDayInWeek(w, d).data(), macw, this);
    existingSessionsWidget = new ExistingSessionsWidget(td->getWeekMap()->getDayInWeek(w, d).data(), macw, this);
    myRoutesWidget = new MyRoutesWidget(td, macw, this);

    addTab(newSessionWidget, "new");
    addTab(myRoutesWidget, "my routes");
    addTab(existingSessionsWidget, "sessions");

    // enable "new" tab if no saved session available
    // enable "sessions" tab otherwise
    if (td->getWeekMap()->getDayInWeek(w, d)->getNumSessions() == 0) {
        setCurrentWidget(newSessionWidget);
        previousActiveWidget = currentIndex();
    } else {
        setCurrentWidget(existingSessionsWidget);
        // load first session
        existingSessionsWidget->loadSession(0);
        macw->getMapControlWidget()->setEnabled(false);
        previousActiveWidget = currentIndex();
    }

    this->setFixedWidth(sizeHint().width());

    createConnections();
}

void SessionsWidget::updateMapControl(int aidx) {
    // "new" tab becomes active
    // if "my sessions" was active before, dont reset
    if (aidx==this->indexOf(newSessionWidget)) {
        if (previousActiveWidget == indexOf(existingSessionsWidget)) {
            newSessionWidget->completeReset();
        }
        newSessionWidget->enableControlWidget();
        previousActiveWidget = currentIndex();
    } else if (aidx == this->indexOf(existingSessionsWidget)) {
        existingSessionsWidget->setSessionDisabled();
        previousActiveWidget = currentIndex();
    } else if (aidx == this->indexOf(myRoutesWidget)) {
        if (previousActiveWidget == indexOf(existingSessionsWidget)) {
            mcw->reset();
        }
    }
}

void SessionsWidget::createConnections() {
    // if user goes to another tab
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateMapControl(int)));
}

bool SessionsWidget::isModified() const {
    return existingSessionsWidget->isWindowModified() || newSessionWidget->isWindowModified() || myRoutesWidget->isWindowModified();
}

void SessionsWidget::showExistingSession(int aidx) {
    if (currentIndex() != indexOf(existingSessionsWidget)) {
        setCurrentWidget(existingSessionsWidget);
        previousActiveWidget = currentIndex();
    }
    existingSessionsWidget->loadSession(aidx);
}
