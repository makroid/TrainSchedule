#ifndef KMLEXPORTDIALOG_H
#define KMLEXPORTDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;
class QLineEdit;
class QGridLayout;
class QVBoxLayout;
class QDialogButtonBox;
class QDoubleSpinBox;

class DurationSpinBox;
class MapWidget;

class KmlExportDialog : public QDialog
{
    Q_OBJECT

public:
    KmlExportDialog(MapWidget* mcw, QWidget* parent = 0);

private slots:
    void openKmlFile();
    void writeKmlFile();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    MapWidget* mapWidget;

    QLabel* durationLabel;
    DurationSpinBox* durationSpinBox;
    QLabel* altitudeLabel;
    QDoubleSpinBox* altitudeSpinBox;
    QLabel* rangeLabel;
    QDoubleSpinBox* rangeSpinBox;
    QLabel* fileNameLabel;
    QPushButton* openFileButton;
    QLineEdit* fileNameEdit;
    QDialogButtonBox* saveCancelGroup;
    QGridLayout* topLayout;
    QVBoxLayout* layout;
};

#endif // KMLEXPORTDIALOG_H
