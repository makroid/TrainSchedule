#include "kmlExportDialog.h"

#include <QString>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDoubleSpinBox>

#include "mapWidget.h"
#include "durationSpinBox.h"
#include "kmlParameters.h"

KmlExportDialog::KmlExportDialog(MapWidget* mw, QWidget* parent)
    : QDialog(parent),
      mapWidget(mw)
{
    createWidgets();
    createLayout();
    createConnections();
}

void KmlExportDialog::createWidgets() {
    durationLabel = new QLabel("flight time");
    durationSpinBox = new DurationSpinBox();
    durationSpinBox->setMinimum(10);
    durationSpinBox->setValue(10);
    durationSpinBox->setSingleStep(1);
    altitudeLabel = new QLabel("altitude");
    altitudeSpinBox = new QDoubleSpinBox();
    altitudeSpinBox->setMinimum(100);
    altitudeSpinBox->setMaximum(20000);
    altitudeSpinBox->setValue(1000);
    rangeLabel = new QLabel("range");
    rangeSpinBox = new QDoubleSpinBox();
    rangeSpinBox->setMinimum(100);
    rangeSpinBox->setMaximum(20000);
    rangeSpinBox->setValue(500);
    fileNameLabel = new QLabel("file");
    openFileButton = new QPushButton("open...");
    fileNameEdit = new QLineEdit();
    saveCancelGroup = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
}

void  KmlExportDialog::createLayout() {
    topLayout = new QGridLayout();
    topLayout->addWidget(durationLabel, 0, 0);
    topLayout->addWidget(durationSpinBox, 0, 1);
    topLayout->addWidget(altitudeLabel, 1, 0);
    topLayout->addWidget(altitudeSpinBox, 1, 1);
    topLayout->addWidget(rangeLabel, 2, 0);
    topLayout->addWidget(rangeSpinBox, 2, 1);
    topLayout->addWidget(fileNameLabel, 3, 0);
    topLayout->addWidget(openFileButton, 3, 1);
    topLayout->addWidget(fileNameEdit, 4, 0, 1, 2);

    layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addWidget(saveCancelGroup);

    setLayout(layout);
}

void KmlExportDialog::createConnections() {
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(openKmlFile()));
    connect(saveCancelGroup, SIGNAL(rejected()), this, SLOT(reject()));
    connect(saveCancelGroup, SIGNAL(accepted()), this, SLOT(writeKmlFile()));
}

void KmlExportDialog::openKmlFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                    tr(""));
    if (!fileName.isEmpty()) {
        fileNameEdit->setText(fileName);
    }
}

void KmlExportDialog::writeKmlFile() {
    QString fileName = fileNameEdit->text();
    if (fileName == "") return;

    QFile kmlFile(fileName);
    if ( ! kmlFile.open(QFile::WriteOnly | QFile::Text)) {
        // TODO: warn
        return;
    }

    int duration = durationSpinBox->value();
    double altitude = altitudeSpinBox->value();
    double range = rangeSpinBox->value();

    KmlParameters params(duration, altitude, range);
    QString kmlCode = mapWidget->getMapPage()->evalExportAsKml(params);
    QTextStream out(&kmlFile);
    out << kmlCode;

    kmlFile.close();
    accept();
}
