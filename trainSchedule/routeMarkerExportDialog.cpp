#include "routeMarkerExportDialog.h"

#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QList>

#include "mapWidget.h"
#include "routemarker.h"

RouteMarkerExportDialog::RouteMarkerExportDialog(MapWidget* mw, QWidget* parent)
    : QDialog(parent),
      mapWidget(mw)
{
    createWidgets();
    createLayout();
    createConnections();

    QList<RouteMarker> markerList = mapWidget->getMapPage()->evalGetRouteMarkers();
    for (int i=0; i<markerList.length(); ++i) {
        QString markerText = QString("%1. %2").arg(markerList[i].getRank()).arg(markerList[i].getMarkerText());
        markerTextEdit->append(markerText);
    }
}

void RouteMarkerExportDialog::createWidgets() {
    markerTextEdit = new QTextEdit();
    markerTextEdit->setMinimumSize(170, 100);
    cancelButton = new QPushButton(tr("Cancel"));
    saveButton = new QPushButton(tr("Save..."));
}

void RouteMarkerExportDialog::createLayout() {
    layout = new QGridLayout();
    layout->addWidget(markerTextEdit, 0, 0, 1, 2);
    layout->addWidget(cancelButton, 1, 0);
    layout->addWidget(saveButton, 1, 1);

    setLayout(layout);
}

void RouteMarkerExportDialog::createConnections() {
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(openSaveDialog()));
}

QString RouteMarkerExportDialog::getRouteMarkersFormatted() {
    return markerTextEdit->toPlainText();
}

void RouteMarkerExportDialog::openSaveDialog() {
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr(""));
    if ( ! saveFileName.isEmpty()) {
        QFile saveFile(saveFileName);
        if ( ! saveFile.open(QFile::WriteOnly)) {
            // TODO warn!
        } else {
            QTextStream txtStream(&saveFile);
            txtStream << getRouteMarkersFormatted();
        }
        saveFile.close();
    }
}
