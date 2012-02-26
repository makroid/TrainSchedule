#include "createWeekDialog.h"

#include <QDateEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCheckBox>


CreateWeekDialog::CreateWeekDialog(QWidget* parent)
    : QDialog(parent),
      doRemoveWeek(false)
{
    createWidgets();
    createLayout();
    createConnections();

    newDate(QDate::currentDate());
}

void CreateWeekDialog::createWidgets() {
    dateLabel = new QLabel("Date");
    dateEdit = new QDateEdit(QDate::currentDate());


    weekLabel = new QLabel("Week");
    weekSpinBox = new QSpinBox;
    weekSpinBox->setMinimum(1);
    weekSpinBox->setMaximum(53);

    yearLabel = new QLabel("Year");
    yearSpinBox = new QSpinBox;
    yearSpinBox->setMinimum(2000);
    yearSpinBox->setMaximum(2100);

    removeBox = new QCheckBox("remove");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
}

void CreateWeekDialog::createLayout() {
    topLayout = new QGridLayout;
    topLayout->addWidget(dateLabel, 0, 0);
    topLayout->addWidget(dateEdit, 0, 1);
    topLayout->addWidget(weekLabel, 1, 0);
    topLayout->addWidget(weekSpinBox, 1, 1);
    topLayout->addWidget(yearLabel, 2, 0);
    topLayout->addWidget(yearSpinBox, 2, 1);
    topLayout->addWidget(removeBox, 3, 1);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void CreateWeekDialog::createConnections() {
    connect(dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(newDate(QDate)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(removeBox, SIGNAL(clicked(bool)), this, SLOT(setRemoveWeek(bool)));
}

void CreateWeekDialog::newDate(QDate adate) {
    int year = adate.year();
    int week = adate.weekNumber();

    if(week == 0) return;

    weekSpinBox->setValue(week);
    yearSpinBox->setValue(year);
}

void CreateWeekDialog::setRemoveWeek(bool b) {
    doRemoveWeek = b;
}
