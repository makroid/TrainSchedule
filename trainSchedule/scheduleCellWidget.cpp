#include "scheduleCellWidget.h"

ScheduleCellWidget::ScheduleCellWidget(Week w, QWidget* parent)
    : QWidget(parent),
      week(w),
      plotHeight(40)
{
    repaint();
}
