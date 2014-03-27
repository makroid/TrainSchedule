#ifndef ROUTEMARKEREXPORTDIALOG_H
#define ROUTEMARKEREXPORTDIALOG_H

#include <QDialog>

class QTextEdit;
class QPushButton;
class QGridLayout;

class MapWidget;

class RouteMarkerExportDialog : public QDialog
{
    Q_OBJECT

public:
    RouteMarkerExportDialog(MapWidget* mw, QWidget* parent=0);

private slots:
     void openSaveDialog();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    QString getRouteMarkersFormatted();

    MapWidget* mapWidget;

    QTextEdit* markerTextEdit;
    QPushButton* cancelButton;
    QPushButton* saveButton;
    QGridLayout* layout;

};

#endif // ROUTEMARKEREXPORTDIALOG_H
