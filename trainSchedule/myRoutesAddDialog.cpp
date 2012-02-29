#include "myRoutesAddDialog.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>

MyRoutesAddDialog::MyRoutesAddDialog(QWidget* parent)
    : QDialog(parent)
{
    createWidgets();
    createLayout();
    createConnections();
}

void MyRoutesAddDialog::createWidgets() {
    nameLabel = new QLabel(tr("Name"));
    nameLineEdit = new QLineEdit;
    reverseCheckBox = new QCheckBox("reverse");
    reverseCheckBox->setChecked(false);
    reverseNameLineEdit = new QLineEdit;
    reverseNameLineEdit->setEnabled(false);

    okButton = new QPushButton(tr("Ok"));
    cancelButton = new QPushButton(tr("Cancel"));
}

void MyRoutesAddDialog::createLayout() {
    QGridLayout* topLayout = new QGridLayout;
    topLayout->addWidget(nameLabel, 0, 0);
    topLayout->addWidget(nameLineEdit, 1, 0, 1, 2);
    topLayout->addWidget(reverseCheckBox, 2, 0);
    topLayout->addWidget(reverseNameLineEdit, 3, 0, 1, 2);

    QHBoxLayout* okCancel = new QHBoxLayout;
    okCancel->addWidget(okButton);
    okCancel->addWidget(cancelButton);

    topLayout->addLayout(okCancel, 4, 1, Qt::AlignRight);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);

    this->setLayout(mainLayout);
}

void MyRoutesAddDialog::createConnections() {
    connect(okButton, SIGNAL(clicked()), this, SLOT(checkAndAccept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(reverseCheckBox, SIGNAL(toggled(bool)), reverseNameLineEdit, SLOT(setEnabled(bool)));
}

void MyRoutesAddDialog::checkAndAccept() {
    // check for valid name
    if (nameLineEdit->text().isEmpty() || (reverseCheckBox->isChecked() && reverseNameLineEdit->text().isEmpty())) {
        reject();
    } else {
        accept();
    }
}
