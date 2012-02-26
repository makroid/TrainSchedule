#ifndef MYROUTESWIDGET_H
#define MYROUTESWIDGET_H

#include <QWidget>
#include <QSharedPointer>

#include "trainData.h"
#include "myRoutesTable.h"

class QLabel;
class QPushButton;
class QLineEdit;
class MyRoutesTable;
class QGridLayout;
class MapAndControlWidget;
class MapWidget;
class MapControlWidget;
class QModelIndex;

class MyRoutesWidget : public QWidget
{
    Q_OBJECT

public:
    MyRoutesWidget(QSharedPointer<TrainingData> dt, MapAndControlWidget* const macw, QWidget* parent=0);

public slots:
    void addRouteToMyRoutes();
    void removeRouteFromMyRoutes();

private slots:
    void insertMyRouteIntoMap(QModelIndex);

private:
    QSharedPointer<TrainingData> trainingData;
    MapWidget* const mapWidget;
    MapControlWidget* const mapControlWidget;

    MyRoutesTable* myRoutesTable;
    QPushButton* addButton;
    QPushButton* removeButton;
    QLabel* myRoutesTableLabel;
    QLabel* filterLabel;
    QLineEdit* filterLineEdit;
    QGridLayout* layout;

    void createWidgets();
    void createLayout();
    void createConnections();

    void insertMyRoutesIntoMap(QModelIndex idx);
};

#endif // MYROUTESWIDGET_H
