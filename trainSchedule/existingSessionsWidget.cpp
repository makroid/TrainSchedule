#include "existingSessionsWidget.h"
#include "mapAndControlWidget.h"

ExistingSessionsWidget::ExistingSessionsWidget(DaySchedule* ds, MapAndControlWidget* const macw, QWidget *parent)
    : QWidget(parent),
      daySdl(ds),
      mapWidget(macw->getMapWidget()),
      mapControlWidget(macw->getMapControlWidget())
{
    createWidgets();
    createLayout();
    createConnections();

    for (int i=0; i<daySdl->getNumSessions(); i++) {
       addItemToSessionCombo(i);
    }
}

void ExistingSessionsWidget::createWidgets() {
    sessionLabel = new QLabel(tr("Session"));
    sessionCombo = new QComboBox;

    typeLabel = new QLabel(tr("Type"));
    typeCombo = new QComboBox;
    typeCombo->addItem("run");
    typeCombo->addItem("bike");
   // typeCombo->addItem("other");

    nameLabel = new QLabel(tr("Name"));
    nameLineEdit = new QLineEdit;

    durationLabel = new QLabel(tr("Time"));
    durationSpinBox = new DurationSpinBox();

    distanceLabel = new QLabel(tr("Distance"));
    distanceSpinBox = new QDoubleSpinBox;
    // distance in KM
    distanceSpinBox->setValue(0);
    distanceSpinBox->setMaximum(400);
    mapWidget->page()->mainFrame()->addToJavaScriptWindowObject("QTDistanceSpinBoxExist", distanceSpinBox);

    speedLabel = new QLabel(tr("Speed"));
    speedLineEdit = new QLineEdit;
    speedLineEdit->setEnabled(false);

    descriptionLabel = new QLabel(tr("Comment"));
    descriptionEdit = new QTextEdit();

    editButton = new QPushButton(tr("Edit"));
    applyButton = new QPushButton(tr("Apply"));
    deleteButton = new QPushButton(tr("Delete"));

    setWidgetsEnabled(false);
}

void ExistingSessionsWidget::createLayout() {
    topLayout = new QGridLayout;
    topLayout->addWidget(sessionLabel, 0, 0);
    topLayout->addWidget(sessionCombo, 0, 1);
    topLayout->addWidget(typeLabel, 1, 0);
    topLayout->addWidget(typeCombo, 1, 1);
    topLayout->addWidget(nameLabel, 2, 0);
    topLayout->addWidget(nameLineEdit, 2, 1);
    topLayout->addWidget(durationLabel, 3,0);
    topLayout->addWidget(durationSpinBox, 3, 1);
    topLayout->addWidget(distanceLabel, 4, 0);
    topLayout->addWidget(distanceSpinBox, 4, 1);
    topLayout->addWidget(speedLabel, 5, 0);
    topLayout->addWidget(speedLineEdit, 5, 1);
    topLayout->addWidget(descriptionLabel, 6, 0);
    topLayout->addWidget(descriptionEdit, 7, 0, 1, 2);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(deleteButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    this->setFixedWidth(sizeHint().width());
 }

void ExistingSessionsWidget::createConnections() {
    connect(mapWidget, SIGNAL(loadFinished(bool)), this, SLOT(reloadMap()));
    connect(sessionCombo, SIGNAL(activated(int)), this, SLOT(loadSession(int)));
    connect(daySdl, SIGNAL(newSessionAdded(int)), this, SLOT(addItemToSessionCombo(int)));
    connect(editButton, SIGNAL(clicked()), this, SLOT(setSessionEnabled()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSession()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(saveSession()));
    connect(durationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSpeedTime(int)));
    connect(distanceSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSpeedDist(double)));
}

void ExistingSessionsWidget::updateDistance(double distanceInKM) {
    distanceSpinBox->setValue(distanceInKM);
}

void ExistingSessionsWidget::addItemToSessionCombo(int i) {
    if (i >= sessionCombo->count()) {
        sessionCombo->addItem(daySdl->getSession(i)->getName());
    }
}

void ExistingSessionsWidget::loadSession(int i) {
    if (i == -1) return;

    editButton->setEnabled(true);
    applyButton->setEnabled(true);
    deleteButton->setEnabled(true);

    sessionCombo->setCurrentIndex(i);
    typeCombo->setCurrentIndex(daySdl->getSession(i)->getTypeCode());
    nameLineEdit->setText(daySdl->getSession(i)->getName());
    durationSpinBox->setValue(daySdl->getSession(i)->getDuration().getDurationInSecs());
    distanceSpinBox->setValue(daySdl->getSession(i)->getDistance().dist);
    descriptionEdit->setPlainText(daySdl->getSession(i)->getDescription());

    // is there a route in the map available?
    int routeId = daySdl->getSession(i)->getMapRoute().getMapId();
    if (routeId != -1) {
        mapWidget->loadMapRoute(daySdl->getSession(i)->getMapRoute());
        mapControlWidget->setRecording();
        mapControlWidget->setCurrentRouteId(routeId);
    } else {
        mapControlWidget->reset();
    }
}

void ExistingSessionsWidget::reloadMap() {
    int idx = sessionCombo->currentIndex();
    if (idx == -1) return;

    if (isVisible()) {
        int routeId = daySdl->getSession(idx)->getMapRoute().getMapId();
        if (routeId != -1) {
            mapWidget->loadMapRoute(daySdl->getSession(idx)->getMapRoute());
            mapControlWidget->setRecording();
            mapControlWidget->setCurrentRouteId(routeId);
        }
    }
}

void ExistingSessionsWidget::setWidgetsEnabled(bool enabled) {
    typeCombo->setEnabled(enabled);
    nameLineEdit->setEnabled(enabled);
    durationSpinBox->setEnabled(enabled);
    distanceSpinBox->setEnabled(enabled);
    descriptionEdit->setEnabled(enabled);
    editButton->setEnabled(enabled);
    applyButton->setEnabled(enabled);
    deleteButton->setEnabled(enabled);
}

void ExistingSessionsWidget::setSessionEnabled() {
    setWidgetsEnabled(true);
    mapControlWidget->setEnabled(true);
}

void ExistingSessionsWidget::setSessionDisabled() {
    setWidgetsEnabled(false);
    mapControlWidget->setEnabled(false);
}

void ExistingSessionsWidget::deleteSession() {
    int idx = sessionCombo->currentIndex();
    bool success = daySdl->deleteSession(idx);
    if (success) {
        // clear fields
        // reset map
        // reset sessionCombo
        sessionCombo->removeItem(idx);
        mapControlWidget->reset();
    }
    setWindowModified(true);
}

void ExistingSessionsWidget::saveSession() {
    int idx = sessionCombo->currentIndex();
    Distance dist(distanceSpinBox->value());
    Duration dur(durationSpinBox->value());
    QString descr = descriptionEdit->toPlainText();

    int comboIdx = typeCombo->currentIndex();
    if (comboIdx != -1) {
        daySdl->getSession(idx)->setType(comboIdx);
    }
    daySdl->getSession(idx)->setDistance(dist);
    daySdl->getSession(idx)->setDuration(dur);
    daySdl->getSession(idx)->setDescription(descr);
    daySdl->getSession(idx)->setName(nameLineEdit->text());
    daySdl->update();

    mapWidget->fillMapRoute(daySdl->getSession(idx)->getMapRoute());
    setWindowModified(true);
}

void ExistingSessionsWidget::updateSpeedDist(double KM) {
    int timeInSecs = durationSpinBox->value();
    int idx = sessionCombo->currentIndex();
    if (idx != -1) {
        speedLineEdit->setText(daySdl->getSession(idx)->speed(timeInSecs, KM));
    }
}

void ExistingSessionsWidget::updateSpeedTime(int sec) {
    double distInKM = distanceSpinBox->value();
    int idx = sessionCombo->currentIndex();
    if (idx != -1) {
        speedLineEdit->setText(daySdl->getSession(idx)->speed(sec, distInKM));
    }
}
