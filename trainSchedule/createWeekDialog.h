#ifndef CREATEWEEKDIALOG_H
#define CREATEWEEKDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QDate>

class QDateEdit;
class QLabel;
class QDialogButtonBox;
class QGridLayout;
class QVBoxLayout;
class QCheckBox;

class CreateWeekDialog : public QDialog
{
    Q_OBJECT

public:
    CreateWeekDialog(QWidget* parent=0);

    int getWeek() const { return weekSpinBox->value(); }
    int getYear() const { return yearSpinBox->value(); }
    bool doRemove() const { return doRemoveWeek; }

private slots:
    void newDate(QDate);
    void setRemoveWeek(bool);

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    bool doRemoveWeek;

    QLabel* dateLabel;
    QDateEdit* dateEdit;
    QLabel* weekLabel;
    QSpinBox* weekSpinBox;
    QLabel* yearLabel;
    QSpinBox* yearSpinBox;
    QCheckBox* removeBox;
    QDialogButtonBox* buttonBox;

    QGridLayout* topLayout;
    QVBoxLayout* mainLayout;
};

#endif // CREATEWEEKDIALOG_H
