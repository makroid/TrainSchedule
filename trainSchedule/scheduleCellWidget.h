#ifndef SCHEDULECELLWIDGET_H
#define SCHEDULECELLWIDGET_H

#include <QWidget>

#include "week.h"

class ScheduleCellWidget : public QWidget
{
    Q_OBJECT

public:
    ScheduleCellWidget(Week w, QWidget* parent=0);
    virtual ~ScheduleCellWidget() {}

    int getPlotHeight() const { return plotHeight; }

signals:
    void plotHeightChanged(Week);

protected:
    Week week;
    int plotHeight;
};

#endif // SCHEDULECELLWIDGET_H
