#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

#include "mapWidget.h"

class QPushButton;
class QLineEdit;
class QLabel;
class QDialogButtonBox;
class QHBoxLayout;
class QCheckBox;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(QWidget* parent=0);

    void load();
    bool isModified() const { return modified; }

private slots:
    void toAccept();
    void openMap();
    void initializeMap();
    void openSaveFile();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    bool isMapVisible;
    bool isMapLoaded;

    bool modified;

    QHBoxLayout* layout;
    QLabel* startLocationLabel;
    QLabel* startLatLabel;
    QLabel* startLngLabel;
    QLineEdit* latLineEdit;
    QLineEdit* lngLineEdit;
    QPushButton* mapButton;
    QLabel* scheduleFileLabel;
    QLineEdit* scheduleFileLineEdit;
    QPushButton* openButton;
    QCheckBox* compressCheckBox;
    QDialogButtonBox* saveCancelGroup;

    MapWidget* mapWidget;
};

#endif // CONFIGDIALOG_H
