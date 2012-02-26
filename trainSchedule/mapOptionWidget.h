#ifndef MAPOPTIONWIDGET_H
#define MAPOPTIONWIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;
class QLineEdit;
class MapWidget;

class MapOptionWidget : public QWidget
{
    Q_OBJECT

public:
    MapOptionWidget(MapWidget* mw, QWidget* parent=0);

    MapWidget* mapWidget;
    QPushButton* closeButton;
    QLineEdit* locationLineEdit;
    QLabel* locationLabel;

    void createWidgets();
    void createLayout();
};

#endif // MAPOPTIONWIDGET_H
