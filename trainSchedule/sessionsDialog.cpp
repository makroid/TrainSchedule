#include "sessionsDialog.h"

// TODO: register modifications and ask whether to apply them

SessionsDialog::SessionsDialog(QSharedPointer<TrainingData> td, Week w, int d, QWidget* parent)
    : QDialog(parent),
      trainingData(td),
      week(w),
      day(d)

{
    setWindowFlags(Qt::Window);
    createWidgets();
    createLayout();
}

void SessionsDialog::createWidgets() {
    mapAndControlWiget = new MapAndControlWidget(this);
    sessionsWidget = new SessionsWidget(trainingData, mapAndControlWiget, week, day, this);
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
