#include "sessionsDialog.h"

// TODO: register modifications and ask whether to apply them

SessionsDialog::SessionsDialog(DaySchedule* ds, QWidget* parent)
    : QDialog(parent),
      daySdl(ds)

{
    setWindowFlags(Qt::Window);
    createWidgets();
    createLayout();
}

void SessionsDialog::createWidgets() {
    mapAndControlWiget = new MapAndControlWidget(this);
    sessionsWidget = new SessionsWidget(daySdl, mapAndControlWiget->getMapWidget(), mapAndControlWiget->getMapControlWidget(), this);
    //mapAndControlWiget->hide();
}

void SessionsDialog::createLayout() {
    layout = new QHBoxLayout;
    layout->addWidget(sessionsWidget);
    layout->addWidget(mapAndControlWiget);
    setLayout(layout);
}

bool SessionsDialog::isModified() const {
    return sessionsWidget->isModified();
}
