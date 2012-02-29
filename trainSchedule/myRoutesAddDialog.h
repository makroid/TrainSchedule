#ifndef MYROUTESADDDIALOG_H
#define MYROUTESADDDIALOG_H

#include <QDialog>

class QWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class MyRoutesAddDialog : public QDialog
{
    Q_OBJECT

public:
    MyRoutesAddDialog(QWidget* parent = 0);

    QLineEdit* nameLineEdit;
    QLineEdit* reverseNameLineEdit;
    QCheckBox* reverseCheckBox;

private slots:
    void checkAndAccept();

private:
    QLabel* nameLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;

    void createWidgets();
    void createLayout();
    void createConnections();
};

#endif // MYROUTESADDDIALOG_H
