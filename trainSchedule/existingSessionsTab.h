#ifndef EXISTINGSESSIONSTAB_H
#define EXISTINGSESSIONSTAB_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QVBoxLayout>

class ExistingSessionsTab : public QWidget
{
    Q_OBJECT

public:
    ExistingSessionsTab(QWidget* parent = 0);

private:
    QLabel* sessionLabel;
    QComboBox* sessionsCombo;
    QLabel* typeLabel;
    QComboBox* typeCombo;
    QLabel* nameLabel;
    QLineEdit* nameLineEdit;
    QLabel* durationLabel;
    DurationSpinBox* durationSpinBox;
    QLabel* distanceLabel;
    QDoubleSpinBox* distanceSpinBox;
    QPushButton *mapButton;
    QDialogButtonBox *dialogButtonGroup;

    QGridLayout* topLayout;
    QVBoxLayout* mainLayout;

    void createWidgets();
    void createLayout();
};

#endif // EXISTINGSESSIONSTAB_H
