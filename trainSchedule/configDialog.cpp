#include "configDialog.h"
#include "configData.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QtDebug>

ConfigDialog::ConfigDialog(QWidget* parent)
    : QDialog(parent),
      isMapVisible(false),
      isMapLoaded(false),
      modified(false)
{
    createWidgets();
    createLayout();
    createConnections();

    load();
}

void ConfigDialog::createWidgets() {
    startLocationLabel = new QLabel(tr("Start Location"));
    startLatLabel = new QLabel("Lat");
    startLngLabel = new QLabel("Lng");
    latLineEdit = new QLineEdit;
    lngLineEdit = new QLineEdit;
    mapButton = new QPushButton(tr("Show Map..."));
    scheduleFileLabel = new QLabel(tr("Schedule File"));
    scheduleFileLineEdit = new QLineEdit;
    openButton = new QPushButton(tr("&Open..."));
    compressCheckBox = new QCheckBox(tr("compress"));
    if (ConfigData::instance().doCompressSdlFile()) {
        qDebug() << "checkstate true";
        compressCheckBox->setCheckState(Qt::Checked);
    }
    saveCancelGroup = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
}

void ConfigDialog::createLayout() {
    QGridLayout* topLayout = new QGridLayout;
    topLayout->addWidget(startLocationLabel, 0, 0, 1, 2);
    topLayout->addWidget(startLatLabel, 1, 0);
    topLayout->addWidget(latLineEdit, 1, 1);
    topLayout->addWidget(startLngLabel, 2, 0);
    topLayout->addWidget(lngLineEdit, 2, 1);
    topLayout->addWidget(mapButton, 3, 1);
    topLayout->addWidget(scheduleFileLabel, 4, 0);
    topLayout->addWidget(scheduleFileLineEdit, 5, 0, 1, 2);
    topLayout->addWidget(openButton, 6, 1);
    topLayout->addWidget(compressCheckBox, 7, 0);

    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLayout);
    leftLayout->addStretch();
    leftLayout->addWidget(saveCancelGroup);

    layout = new QHBoxLayout;
    layout->addLayout(leftLayout);

//    QSizePolicy policy = sizePolicy();
//    policy.setHorizontalPolicy(QSizePolicy::Maximum);
//    setSizePolicy(policy);

    setLayout(layout);
}

void ConfigDialog::createConnections() {
    connect(saveCancelGroup, SIGNAL(accepted()), this, SLOT(toAccept()));
    connect(saveCancelGroup, SIGNAL(rejected()), this, SLOT(reject()));
    connect(mapButton, SIGNAL(clicked()), this, SLOT(openMap()));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openSaveFile()));
    //connect(compressCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setCompress(int)));
}

void ConfigDialog::toAccept() {

    // TODO: test for spaces, test for changes in fields
    if ( ! scheduleFileLineEdit->text().isEmpty()) {
        ConfigData::instance().setScheduleFileName(scheduleFileLineEdit->text());
    }
    // TODO: test for valid inputs
    if ( ! latLineEdit->text().isEmpty()) {
        ConfigData::instance().setStartLat(latLineEdit->text().toDouble());
    }
    if ( ! lngLineEdit->text().isEmpty()) {
        ConfigData::instance().setStartLng(lngLineEdit->text().toDouble());
    }
    if (compressCheckBox->isChecked() != ConfigData::instance().doCompressSdlFile()) {
        qDebug() << "compression status changed";
        if (compressCheckBox->isChecked()) {
            ConfigData::instance().setCompressSdlFile(true);
        } else {
            ConfigData::instance().setCompressSdlFile(false);
        }
        // if modified==true, schedule must be saved again
        // because file format has changed
        modified = true;
    }

    qDebug() << "calling ConfigDate::writeXML()";
    ConfigData::instance().writeXML();
    accept();
}

void ConfigDialog::load() {
    ConfigData::instance().readXML();

    double lat = ConfigData::instance().getStartLocation().x();
    double lng = ConfigData::instance().getStartLocation().y();
    QString fileName = ConfigData::instance().getScheduleFileName();
    latLineEdit->setText(QString::number(lat, 'f', 6));
    lngLineEdit->setText(QString::number(lng, 'f', 6));
    scheduleFileLineEdit->setText(fileName);
}

void ConfigDialog::openMap() {
    if ( ! isMapLoaded) {
        mapWidget = new MapWidget(this);
        // this connection must be set up here, when mapWidget is initialized
        connect(mapWidget, SIGNAL(loadFinished(bool)), this, SLOT(initializeMap()));
        mapWidget->load(QUrl("mapsTest.html"));
        mapWidget->hide();
        layout->insertWidget(-1, mapWidget);
        isMapLoaded = true;
    }
    if ( ! isMapVisible) {
        mapButton->setText("Close Map");
        mapWidget->show();
    } else {
        mapButton->setText("Open Map...");
        mapWidget->hide();
        adjustSize();
    }
    isMapVisible = !isMapVisible;
}

void ConfigDialog::openSaveFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                    tr(""));
    if (!fileName.isEmpty()) {
        scheduleFileLineEdit->setText(fileName);
    }
}

void ConfigDialog::initializeMap() {
    mapWidget->getMapPage()->mainFrame()->addToJavaScriptWindowObject("QTLatLineEdit", latLineEdit);
    mapWidget->getMapPage()->mainFrame()->addToJavaScriptWindowObject("QTLngLineEdit", lngLineEdit);
    mapWidget->getMapPage()->evalSetCenter();
    mapWidget->getMapPage()->mainFrame()->evaluateJavaScript("initializeStartPosition();");
}
